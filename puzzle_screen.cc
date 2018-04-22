#include "puzzle_screen.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

#include "util.h"

PuzzleScreen::PuzzleScreen() :
  gui_("gui.png", 3, 8, 8),
  panel_("panel.png", 2, 128, 128),
  bg_("starfield.png", 256, 2048, 8),
  text_("text.png"),
  state_(GameState::Playing),
  timer_(0), state_timer_(0),
  bgoffset_(0), choice_(0)
{
  rand_.seed(Util::random_seed());
}

bool PuzzleScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (state_ == GameState::Playing) {
    timer_ += elapsed;
    bgoffset_ -= elapsed;

    int tx = 0, ty = 0;
    if (input.key_held(Input::Button::Left)) --tx;
    if (input.key_held(Input::Button::Right)) ++tx;
    if (input.key_held(Input::Button::Up)) --ty;
    if (input.key_held(Input::Button::Down)) ++ty;
    player_.thrust(tx, ty);

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

    // TODO Consolidate these objects into one
    for (auto& bullet : bullets_) {
      bullet->update(elapsed);
    }

    for (auto& explosion : explosions_) {
      explosion.update(elapsed);
    }

    for (auto& powerup : powerups_) {
      powerup.update(elapsed);

      for (auto& bullet : bullets_) {
        if (collision(*bullet, powerup, 10)) {
          if (bullet->type() == Bullet::Type::Laser && powerup.rotate()) audio.play_sample("rotate.wav");
          bullet->kill();
        }
      }
    }
  }

  if (state_ == GameState::Playing) {
    std::uniform_real_distribution<double> p(0, 1);
    if (p(rand_) < 0.01) {
      powerups_.emplace_back();
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

          default:
            break;
        }
      }
    }
  }

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

      player_.thrust(tx, ty);
    } else {
      if (!handle_menu(input, audio)) return false;
    }
  }

  return true;
}

void PuzzleScreen::draw(Graphics& graphics) const {
  bg_.draw(graphics, 0, bgoffset_);

  player_.draw(graphics);
  for (const auto& powerup : powerups_) {
    powerup.draw(graphics);
  }
  for (const auto& bullet : bullets_) {
    bullet->draw(graphics);
  }
  for (const auto& explosion: explosions_) {
    explosion.draw(graphics);
  }

  SDL_Rect r = { 184, 0, 72, 240 };
  graphics.draw_rect(&r, 0x000000ff, true);

  const int s = (timer_ / 1000) % 60;
  const int m = timer_ / 1000 / 60;

  if (state_ != GameState::Paused) puzzle_.draw(graphics, 188, 4);

  if (state_ == GameState::Playing) {
    const int wt = player_.weapon_timer();
    if (wt > 0) {
      text_.draw(graphics, std::to_string(wt), 92, 224, Text::Alignment::Center);
    }
  }

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

  if (state_ == GameState::Paused) {
    if ((state_timer_ / 500) % 2 == 0) {
      text_.draw(graphics, "Paused", 92, 112, Text::Alignment::Center);
    }

    text_.draw(graphics, "Resume", 92, 136, Text::Alignment::Center);
    text_.draw(graphics, "Main Menu", 92, 152, Text::Alignment::Center);
    text_.draw(graphics, ">           <", 92, 136 + 16 * choice_, Text::Alignment::Center);
  }

  if (state_ == GameState::Victory) {
    SDL_Rect r = { 0, 0, 184, 240 };
    int white = Util::clamp(state_timer_ / 20, 0, 256);
    graphics.draw_rect(&r, 0xffffff00 + white, true);

    if (state_timer_ >= 5000) {
      panel_.draw(graphics, 0, 28, 58);
      text_.draw(graphics, "Retry", 92, 136, Text::Alignment::Center);
      text_.draw(graphics, "Main Menu", 92, 152, Text::Alignment::Center);
      text_.draw(graphics, ">           <", 92, 136 + 16 * choice_, Text::Alignment::Center);
    }
  }

  if (state_ == GameState::Defeat) {
    SDL_Rect r = { 0, 0, 184, 240 };
    int red = Util::clamp(state_timer_ / 10, 0, 196);
    graphics.draw_rect(&r, 0xdd222200 + red, true);
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
    audio.play_sample("whirl.wav");
  }
}

void PuzzleScreen::resume(Audio& audio) {
  if (state_ == GameState::Paused) {
    state_ = GameState::Playing;
    audio.play_sample("whirl.wav");
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
