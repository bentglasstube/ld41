#include "title_screen.h"

#include "puzzle_screen.h"

TitleScreen::TitleScreen() : text_("text.png"), timer_(0) {}

bool TitleScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  timer_ = (timer_ + elapsed) % 1000;
  return !input.any_pressed();
}

void TitleScreen::draw(Graphics& graphics) const {
  if (timer_ < 500) text_.draw(graphics, "Press any key", 128, 220, Text::Alignment::Center);
}

Screen* TitleScreen::next_screen() const {
  return new PuzzleScreen();
}
