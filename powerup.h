#pragma once

#include "graphics.h"
#include "spritemap.h"

class Powerup {
  public:

    enum class Type { Right, Left, Up, Down, K, N, O, B };

    Powerup();
    void update(unsigned int elapased);
    void draw(Graphics& graphics) const;

    bool dead() const;

  private:

    SpriteMap sprites_;
    double x_, y_, vy_;
    Type type_;
};
