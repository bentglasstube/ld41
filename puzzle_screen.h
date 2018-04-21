#pragma once

#include <array>
#include <random>
#include <vector>

#include "screen.h"
#include "spritemap.h"
#include "text.h"

#include "powerup.h"
#include "puzzle.h"
#include "ship.h"

class PuzzleScreen : public Screen {
  public:
    PuzzleScreen();

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;

  private:

    SpriteMap gui_;
    Text text_;
    std::mt19937 rand_;

    Puzzle puzzle_;
    Ship player_;
    std::vector<Powerup> powerups_;

    int timer_;

    void reset();
};
