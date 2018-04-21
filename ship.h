#pragma once

#include "graphics.h"
#include "spritemap.h"

class Ship {
  public:

    Ship();

    void thrust(double v);
    void update(unsigned int elapsed);
    void draw(Graphics& graphics, int y) const;

    double x() const { return x_; }

  private:

    SpriteMap sprites_;
    double x_, vx_;

    static constexpr double kSpeed = 0.05;
};
