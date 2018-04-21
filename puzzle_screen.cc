#include "puzzle_screen.h"

PuzzleScreen::PuzzleScreen() :
  gui_("gui.png", 3, 8, 8),
  text_("text.png")
{
  reset();
}

bool PuzzleScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  if (input.key_pressed(Input::Button::A)) reset();

  if (!puzzle_.solved()) {
    timer_ += elapsed;

    if (input.key_pressed(Input::Button::Right)) {
      puzzle_.move(Puzzle::Direction::Left);
    } else if (input.key_pressed(Input::Button::Left)) {
      puzzle_.move(Puzzle::Direction::Right);
    } else if (input.key_pressed(Input::Button::Up)) {
      puzzle_.move(Puzzle::Direction::Down);
    } else if (input.key_pressed(Input::Button::Down)) {
      puzzle_.move(Puzzle::Direction::Up);
    }
  }

  return true;
}

void PuzzleScreen::draw(Graphics& graphics) const {
  puzzle_.draw(graphics, 188, 4);

  const int s = (timer_ / 1000) % 60;
  const int m = timer_ / 1000 / 60;

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
