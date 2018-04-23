#include "puzzle_screen.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

#include "util.h"

#include "bar.h"

PuzzleScreen::PuzzleScreen() :
  gui_("gui.png", 3, 8, 8),
  panel_("panel.png", 2, 48, 64),
  bg_("starfield.png", 256, 2048, 8),
  text_("text.png"),
  state_(GameState::Playing),
  timer_(0), state_timer_(0), enemy_timer_(1500),
  flash_timer_(0), bgoffset_(0), choice_(0)
{
  rand_.seed(Util::random_seed());
}

bool PuzzleScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (state_ == GameState::Playing) {
    if (!audio.music_playing()) audio.play_music("spaceloop.ogg");

    timer_ += elapsed;
    enemy_timer_ -= elapsed;
    bgoffset_ -= elapsed;
    if (flash_timer_ > 0) flash_timer_ -= elapsed;

    int tx = 0, ty = 0;
    if (input.key_held(Input::Button::Left)) --tx;
    if (input.key_held(Input::Button::Right)) ++tx;
    if (input.key_held(Input::Button::Up)) --ty;
    if (input.key_held(Input::Button::Down)) ++ty;
    player_.thrust(tx, ty, input.key_held(Input::Button::B));

    if (input.key_pressed(Input::Button::A) && bullets_.size() < 3) {
      Bullet* b = player_.fire();
      if (b) {
        bullets_.push_back(std::move(b));
        audio.play_sample(b->audio_sample());
      }
    }
  }

  if (state_ != GameState::Paused) {
    player_.update(elapsed);
    if (player_.boosting()) audio.play_sample("boost.wav");

    for (auto& bullet : bullets_) bullet->update(elapsed);
    for (auto& enemy : enemies_) {
      if (!player_.dead() && enemy.fire()) {
        fireballs_.emplace_back(enemy.x(), enemy.y(), Enemy::Type::Fireball, player_);
      }

      enemy.update(elapsed);

      for (auto& bullet : bullets_) {
        if (collision(*bullet, enemy, 10)) {
          explosions_.emplace_back(enemy.x(), enemy.y());
          powerups_.emplace_back(enemy.x(), enemy.y());
          audio.play_sample("explode.wav");

          bullet->kill();
          enemy.kill();
        }
      }

      if (state_ == GameState::Playing && collision(player_, enemy, 12)) {
        player_.hurt(audio);
        explosions_.emplace_back(enemy.x(), enemy.y());
        enemy.kill();
      }
    }

    for (auto& explosion : explosions_) explosion.update(elapsed);
    for (auto& fireball: fireballs_) {
      fireball.update(elapsed);
      if (player_.dead()) continue;
      if (collision(fireball, player_, 12)) {
        player_.hurt(audio);
        explosions_.emplace_back(player_.x(), player_.y());
        fireball.kill();
      }
    }

    for (auto& powerup : powerups_) {
      powerup.update(elapsed);

      for (auto& bullet : bullets_) {
        if (collision(*bullet, powerup, 10)) {
          if (bullet->type() == Bullet::Type::Laser && powerup.rotate()) {
            audio.play_sample("rotate.wav");
            bullet->kill();
          }
        }
      }
    }
  }

  if (state_ == GameState::Playing) {
    if (player_.dead()) lose(audio);

    if (enemy_timer_ < 0) {
      // TODO better enemy progression
      std::uniform_int_distribution<int> tdist(0, 5);
      std::uniform_int_distribution<int> xdist(8, 176);
      std::uniform_int_distribution<int> ydist(8, 96);
      enemies_.emplace_back(xdist(rand_), ydist(rand_), static_cast<Enemy::Type>(tdist(rand_)), player_);

      enemy_timer_ += std::max(3000 - timer_ / 250, 1000);
    }

    for (auto& powerup : powerups_) {
      if (collision(powerup, player_, 12)) {
        powerup.kill();

        switch (powerup.type()) {
          case Powerup::Type::Left:
            if (puzzle_.move(Puzzle::Direction::Right)) audio.play_sample("slide.wav");
            break;

          case Powerup::Type::Right:
            if (puzzle_.move(Puzzle::Direction::Left)) audio.play_sample("slide.wav");
            break;

          case Powerup::Type::Up:
            if (puzzle_.move(Puzzle::Direction::Down)) audio.play_sample("slide.wav");
            break;

          case Powerup::Type::Down:
            if (puzzle_.move(Puzzle::Direction::Up)) audio.play_sample("slide.wav");
            break;

          case Powerup::Type::K:
            player_.weapon(Bullet::Type::Laser);
            audio.play_sample("powerup.wav");
            break;

          case Powerup::Type::N:
            player_.get_shield();
            audio.play_sample("powerup.wav");
            break;

          case Powerup::Type::O:
            player_.get_fuel();
            audio.play_sample("powerup.wav");
            break;

          case Powerup::Type::B:
            for (auto& enemy : enemies_) {
              explosions_.emplace_back(enemy.x(), enemy.y());
              enemy.kill();
            }
            audio.play_sample("powerup.wav");
            flash_timer_ = 250;
            break;

          default:
            break;
        }
      }
    }
  }

  explosions_.erase(std::remove_if(
        explosions_.begin(), explosions_.end(),
        [](const Explosion& e){ return e.dead();}),
      explosions_.end());

  enemies_.erase(std::remove_if(
        enemies_.begin(), enemies_.end(),
        [](const Enemy& e){ return e.dead();}),
      enemies_.end());

  powerups_.erase(std::remove_if(
        powerups_.begin(), powerups_.end(),
        [](const Powerup& p){ return p.dead();}),
      powerups_.end());

  bullets_.erase(std::remove_if(
        bullets_.begin(), bullets_.end(),
        [](const Bullet* const b){ return b->dead();}),
      bullets_.end());

  if (state_ == GameState::Playing) {
    if (puzzle_.solved()) win(audio);
    else if (input.key_pressed(Input::Button::Start)) pause(audio);
  } else if (state_ == GameState::Paused) {
    state_timer_ += elapsed;

    if (!handle_menu(input, audio)) {
      if (choice_ == 0) resume(audio);
      if (choice_ == 1) return false;
    }

  } else if (state_ == GameState::Victory) {
    if (state_timer_ < 5000) {
      state_timer_ += elapsed;
      bgoffset_ -= elapsed * (state_timer_ / 100);

      int tx = 0;
      if (player_.x() < 91) ++tx;
      if (player_.x() > 93) --tx;

      int ty = 0;
      if (player_.y() < 199) ++ty;
      if (player_.y() > 201) --ty;
      if (state_timer_ > 2500) ty = -4;

      player_.thrust(tx, ty, false);
    } else {
      if (!handle_menu(input, audio)) return false;
    }
  } else if (state_ == GameState::Defeat) {
    state_timer_ += elapsed;

    if (state_timer_ > 1500) {
      if (!handle_menu(input, audio)) return false;
    }
  }

  return true;
}

void PuzzleScreen::draw(Graphics& graphics) const {
  bg_.draw(graphics, 0, bgoffset_);

  for (const auto& powerup : powerups_) powerup.draw(graphics);
  for (const auto& bullet : bullets_) bullet->draw(graphics);
  for (const auto& explosion : explosions_) explosion.draw(graphics);
  for (const auto& enemy : enemies_) enemy.draw(graphics);
  for (const auto& fireball: fireballs_) fireball.draw(graphics);

  SDL_Rect r = { 184, 0, 72, 240 };
  graphics.draw_rect(&r, 0x000000ff, true);

  const int s = (timer_ / 1000) % 60;
  const int m = timer_ / 1000 / 60;

  if (state_ != GameState::Paused) puzzle_.draw(graphics, 188, 4);

  std::ostringstream timer;
  timer << m << ":" << std::setw(2) << std::setfill('0') << s;
  text_.draw(graphics, timer.str(), 252, 71, Text::Alignment::Right);

  gui_.draw(graphics, 0, 184, 0);
  gui_.draw(graphics, 2, 248, 0);
  gui_.draw(graphics, 9, 184, 82);
  gui_.draw(graphics, 11, 248, 82);

  for (int x = 192; x < 248; x += 8) {
    gui_.draw(graphics, 1, x, 0);
    gui_.draw(graphics, 1, x, 68);
    gui_.draw(graphics, 1, x, 86);
  }

  for (int y = 8; y < 80; y += 8) {
    if (y == 64) y -= 4;
    gui_.draw(graphics, 3, 184, y);
    gui_.draw(graphics, 5, 248, y);
  }

  gui_.draw(graphics, 6, 184, 66);
  gui_.draw(graphics, 8, 248, 66);

  Bar health_bar = Bar(Bar::Color::Red, 16, 72);
  Bar weapon_bar = Bar(Bar::Color::Green, 25, 72);
  Bar shield_bar = Bar(Bar::Color::Blue, 30000, 72);
  Bar boost_bar = Bar(Bar::Color::Yellow, 5000, 72);

  health_bar.draw(graphics, 184, 96, player_.health());
  weapon_bar.draw(graphics, 184, 104, player_.weapon_shots());
  shield_bar.draw(graphics, 184, 112, player_.shield());
  boost_bar.draw(graphics, 184, 120, player_.fuel());

  player_.draw(graphics);

  if (flash_timer_ > 0) {
    SDL_Rect r = { 0, 0, 184, 240 };
    const int amt = std::abs(flash_timer_ - 125);
    const int flash = Util::clamp(2 * (125 - amt), 0, 255);
    graphics.draw_rect(&r, 0xffffff00 + flash, true);
  }

  if (state_ == GameState::Paused) {
    if ((state_timer_ / 500) % 2 == 0) {
      text_.draw(graphics, "Paused", 92, 112, Text::Alignment::Center);
    }

    text_.draw(graphics, "Resume", 92, 136, Text::Alignment::Center);
    text_.draw(graphics, "Main Menu", 92, 152, Text::Alignment::Center);
    text_.draw(graphics, ">           <", 92, 136 + 16 * choice_, Text::Alignment::Center);
  } else if (state_ == GameState::Victory) {
    SDL_Rect r = { 0, 0, 184, 240 };
    int white = Util::clamp(state_timer_ / 15, 0, 255);
    graphics.draw_rect(&r, 0xffffff00 + white, true);

    if (state_timer_ >= 5000) {
      panel_.draw(graphics, 0, 68, 58);
      text_.draw(graphics, "Retry", 92, 136, Text::Alignment::Center);
      text_.draw(graphics, "Main Menu", 92, 152, Text::Alignment::Center);
      text_.draw(graphics, ">           <", 92, 136 + 16 * choice_, Text::Alignment::Center);
    }
  } else if (state_ == GameState::Defeat) {
    SDL_Rect r = { 0, 0, 184, 240 };
    int fade = Util::clamp(state_timer_ / 10, 0, 196);
    graphics.draw_rect(&r, 0x22000000 + fade, true);

    if (state_timer_ > 1500) {
      panel_.draw(graphics, 1, 68, 58);
      text_.draw(graphics, "Retry", 92, 136, Text::Alignment::Center);
      text_.draw(graphics, "Main Menu", 92, 152, Text::Alignment::Center);
      text_.draw(graphics, ">           <", 92, 136 + 16 * choice_, Text::Alignment::Center);
    }
  }
}

Screen* PuzzleScreen::next_screen() const {
  if (choice_ == 0) {
    PuzzleScreen* p = new PuzzleScreen;
    p->reset(puzzle_.difficulty());
    return p;
  } else {
    return new TitleScreen();
  }
}

void PuzzleScreen::reset(Puzzle::Difficulty diff) {
  timer_ = 0;
  puzzle_.shuffle(diff);
}

bool PuzzleScreen::collision(const Object& a, const Object& b, double r) const {
  const double dx = a.x() - b.x();
  const double dy = a.y() - b.y();
  return dx * dx + dy * dy < r * r;
}

void PuzzleScreen::pause(Audio& audio) {
  if (state_ == GameState::Playing) {
    state_ = GameState::Paused;
    state_timer_ = 0;
    choice_ = 0;
    audio.play_sample("pause.wav");
    audio.music_volume(3);
  }
}

void PuzzleScreen::resume(Audio& audio) {
  if (state_ == GameState::Paused) {
    state_ = GameState::Playing;
    audio.play_sample("pause.wav");
    audio.music_volume(10);
  }
}

void PuzzleScreen::win(Audio& audio) {
  if (state_ == GameState::Playing) {
    state_ = GameState::Victory;
    state_timer_ = 0;
    choice_ = 0;
    audio.play_sample("ditty.wav");
  }
}

void PuzzleScreen::lose(Audio& audio) {
  if (state_ == GameState::Playing) {
    state_ = GameState::Defeat;
    state_timer_ = 0;
    choice_ = 0;

    explosions_.emplace_back(player_.x(), player_.y());
    audio.play_sample("explode.wav");
  }
}

bool PuzzleScreen::handle_menu(const Input& input, Audio& audio) {
  if (input.key_pressed(Input::Button::Up)) {
    if (choice_ > 0) {
      --choice_;
      audio.play_sample("select.wav");
    }
  }
  if (input.key_pressed(Input::Button::Down)) {
    if (choice_ < 1) {
      choice_ = 1;
      audio.play_sample("select.wav");
    }
  }
  if (input.key_pressed(Input::Button::Select)) {
    choice_ = 1 - choice_;
    audio.play_sample("select.wav");
  }

  if (input.key_pressed(Input::Button::Start)) return false;
  if (input.key_pressed(Input::Button::A)) return false;
  return true;
}
