#include "puzzle_screen.h"

#include <algorithm>

#include "util.h"

PuzzleScreen::PuzzleScreen() :
  gui_("gui.png", 3, 8, 8),
  bg_("starfield.png", 256, 2048, 8),
  text_("text.png")
{
  rand_.seed(Util::random_seed());
  reset();
}

bool PuzzleScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (!puzzle_.solved()) {
    timer_ += elapsed;
  }

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
      audio.play_sample("bullet.wav");
    }
  }

  player_.update(elapsed);

  std::uniform_real_distribution<double> p(0, 1);
  if (p(rand_) < 0.01) {
    powerups_.emplace_back();
  }

  // TODO Consolidate these objects into one
  for (auto& bullet : bullets_) {
    bullet->update(elapsed);
  }

  for (auto& explosion : explosions_) {
    explosion.update(elapsed);
  }

  for (auto& powerup : powerups_) {
    powerup.update(elapsed);

    if (collision(powerup, player_, 12)) {
      powerup.kill();
      audio.play_sample("powerup.wav");

      switch (powerup.type()) {
        case Powerup::Type::Left:
          puzzle_.move(Puzzle::Direction::Right);
          break;

        case Powerup::Type::Right:
          puzzle_.move(Puzzle::Direction::Left);
          break;

        case Powerup::Type::Up:
          puzzle_.move(Puzzle::Direction::Down);
          break;

        case Powerup::Type::Down:
          puzzle_.move(Puzzle::Direction::Up);
          break;

        case Powerup::Type::K:
          player_.weapon(Bullet::Type::Laser);
          break;

        default:
          break;
      }
    }

    for (auto& bullet : bullets_) {
      if (collision(*bullet, powerup, 10)) {
        if (bullet->type() == Bullet::Type::Laser && powerup.rotate()) audio.play_sample("rotate.wav");
        bullet->kill();
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

  return true;
}

void PuzzleScreen::draw(Graphics& graphics) const {
  bg_.draw(graphics, 0, -timer_);

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

  puzzle_.draw(graphics, 188, 4);

  // TODO use fancy new string nonsense
  char buffer[24];
  sprintf(buffer, "%u:%02u", m, s);
  text_.draw(graphics, buffer, 252, 71, Text::Alignment::Right);

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
}

Screen* PuzzleScreen::next_screen() const {
  return nullptr;
}

void PuzzleScreen::reset() {
  timer_ = 0;
  puzzle_.shuffle();
}

bool PuzzleScreen::collision(const Object& a, const Object& b, double r) const {
  const double dx = a.x() - b.x();
  const double dy = a.y() - b.y();
  return dx * dx + dy * dy < r * r;
}
