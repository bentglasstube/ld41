#pragma once

#include "audio.h"
#include "backdrop.h"
#include "graphics.h"
#include "input.h"
#include "screen.h"
#include "text.h"

class TitleScreen : public Screen {
  public:

    TitleScreen();

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;

  private:

    Backdrop bg_;
    Text text_;
    int choice_;
};
