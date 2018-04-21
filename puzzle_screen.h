#pragma once

#include <array>

#include "screen.h"
#include "spritemap.h"
#include "text.h"

class PuzzleScreen : public Screen {
  public:
    PuzzleScreen();

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;

  private:

    enum class Direction { Left, Right, Up, Down };

    SpriteMap tiles_;
    Text text_;
    int timer_;
    bool solved_;
    std::array<unsigned int, 16> pieces_;

    void reset();
    void move(Direction direction);
};
