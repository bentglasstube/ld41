#include "puzzle_screen.h"

#include <algorithm>
#include <random>

#include "util.h"

PuzzleScreen::PuzzleScreen() :
  tiles_("puzzle.png", 4, 16, 16),
  gui_("gui.png", 3, 8, 8),
  text_("text.png")
{
  reset();
}

bool PuzzleScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  if (input.key_pressed(Input::Button::A)) reset();

  if (!solved_) {
    timer_ += elapsed;

    if (input.key_pressed(Input::Button::Right)) {
      move(Direction::Left);
    } else if (input.key_pressed(Input::Button::Left)) {
      move(Direction::Right);
    } else if (input.key_pressed(Input::Button::Up)) {
      move(Direction::Down);
    } else if (input.key_pressed(Input::Button::Down)) {
      move(Direction::Up);
    }
  }

  return true;
}

void PuzzleScreen::draw(Graphics& graphics) const {
  for (int i = 0; i < 16; ++i) {
    const int x = (i % 4) * 16 + 188;
    const int y = (i / 4) * 16 + 4;

    const int n = (pieces_[i] == 15 && !solved_) ? 0 : pieces_[i] + 4;
    tiles_.draw(graphics, n, x, y);
  }

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
  for (size_t i = 0; i < 16; ++i) {
    pieces_[i] = i;
  }

  std::mt19937 rand;
  std::uniform_int_distribution<int> dist(0, 3);

  rand.seed(Util::random_seed());

  for (size_t i = 0; i < 4096; ++i) {
    move(static_cast<PuzzleScreen::Direction>(dist(rand)));
  }

  timer_ = 0;
  solved_ = false;
}

void PuzzleScreen::move(PuzzleScreen::Direction direction) {
  int empty = 0;

  for (size_t i = 0; i < 16; ++ i) {
    if (pieces_[i] == 15) {
      empty = i;
      break;
    }
  }

  switch (direction) {
    case Direction::Left:
      if (empty % 4 == 0) return;
      std::swap(pieces_[empty - 1], pieces_[empty]);
      break;

    case Direction::Right:
      if (empty % 4 == 3) return;
      std::swap(pieces_[empty + 1], pieces_[empty]);
      break;

    case Direction::Up:
      if (empty < 4) return;
      std::swap(pieces_[empty - 4], pieces_[empty]);
      break;

    case Direction::Down:
      if (empty > 11) return;
      std::swap(pieces_[empty + 4], pieces_[empty]);
      break;
  }

  for (size_t i = 0; i < 16; ++i) {
    if (i != pieces_[i]) return;
  }

  solved_ = true;
}
