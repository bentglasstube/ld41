#pragma once

#include <array>

#include "graphics.h"
#include "spritemap.h"

class Puzzle {
  public:

    enum class Direction { Left, Right, Up, Down };
    enum class Difficulty { Casual, Hardcore, GoodLuckFucker };

    Puzzle();

    void shuffle(Difficulty diff);
    bool move(Direction dir);
    void random_move(size_t count = 1);
    bool solved() const;
    void draw(Graphics& graphics, int x, int y) const;

    inline Difficulty difficulty() const { return difficulty_; };

  private:

    SpriteMap tiles_;
    Difficulty difficulty_;
    std::array<unsigned int, 16> pieces_;
};


