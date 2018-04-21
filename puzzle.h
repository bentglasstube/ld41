#pragma once

#include <array>

#include "graphics.h"
#include "spritemap.h"

class Puzzle {
  public:

    enum class Direction { Left, Right, Up, Down };

    Puzzle();

    bool move(Direction dir);
    bool solved() const;
    void shuffle();
    void draw(Graphics& graphics, int x, int y) const;

  private:

    SpriteMap tiles_;
    std::array<unsigned int, 16> pieces_;
};


