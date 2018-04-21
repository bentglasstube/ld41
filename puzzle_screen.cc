#include "puzzle_screen.h"

#include <algorithm>

#include "util.h"

PuzzleScreen::PuzzleScreen() :
  gui_("gui.png", 3, 8, 8),
  text_("text.png")
{
  rand_.seed(Util::random_seed());
  reset();
}

bool PuzzleScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  if (!puzzle_.solved()) {
    timer_ += elapsed;
  }

  int t = 0;
  if (input.key_held(Input::Button::Left)) --t;
  if (input.key_held(Input::Button::Right)) ++t;
  player_.thrust(t);

  player_.update(elapsed);

  std::uniform_real_distribution<double> p(0, 1);
  if (p(rand_) < 0.01) {
    powerups_.emplace_back();
  }

  for (auto& powerup : powerups_) {
    powerup.update(elapsed);

    if (powerup.touching(player_.x(), 208)) {
      powerup.kill();

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

      }
    }
  }

  powerups_.erase(std::remove_if(
        powerups_.begin(), powerups_.end(),
        [](const Powerup& p){ return p.dead();}),
      powerups_.end());

  return true;
}

void PuzzleScreen::draw(Graphics& graphics) const {
  puzzle_.draw(graphics, 188, 4);
  player_.draw(graphics, 208);

  for (const auto& powerup : powerups_) {
    powerup.draw(graphics);
  }

  const int s = (timer_ / 1000) % 60;
  const int m = timer_ / 1000 / 60;

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
