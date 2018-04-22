#include "title_screen.h"

#include "puzzle_screen.h"

TitleScreen::TitleScreen() : bg_("title.png"), text_("text.png"), choice_(0) {}

bool TitleScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (input.key_pressed(Input::Button::Up)) {
    if (choice_ > 0) {
      --choice_;
      audio.play_sample("select.wav");
    }
  }

  if (input.key_pressed(Input::Button::Down)) {
    if (choice_ < 3) {
      ++choice_;
      audio.play_sample("select.wav");
    }
  }

  if (input.key_pressed(Input::Button::Select)) {
    choice_ = (choice_ + 1) % 4;
    audio.play_sample("select.wav");
  }

  if (input.key_pressed(Input::Button::A)) return false;
  if (input.key_pressed(Input::Button::Start)) return false;

  return true;
}

void TitleScreen::draw(Graphics& graphics) const {
  bg_.draw(graphics);

  text_.draw(graphics, "Casual", 128, 128, Text::Alignment::Center);
  text_.draw(graphics, "Hardcore", 128, 144, Text::Alignment::Center);
  text_.draw(graphics, "Good Luck, Fucker", 128, 160, Text::Alignment::Center);
  text_.draw(graphics, "Quit", 128, 176, Text::Alignment::Center);

  text_.draw(graphics, ">                   <", 128, 128 + choice_ * 16, Text::Alignment::Center);
}

Screen* TitleScreen::next_screen() const {
  if (choice_ == 3) return nullptr;

  PuzzleScreen* p = new PuzzleScreen();
  p->reset(static_cast<Puzzle::Difficulty>(choice_));
  return p;
}
