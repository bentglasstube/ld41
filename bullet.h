#pragma once

#include "graphics.h"
#include "spritemap.h"

class Bullet {
  public:

    Bullet(double x, double y);

    void update(unsigned int elapsed);
    void draw(Graphics& graphics) const;

    inline bool dead() const { return y_ < 0; };
    inline void kill() { y_ = -999; };

    inline double x() const { return x_; }
    inline double y() const { return y_; }

  private:

    SpriteMap sprites_;
    double x_, y_, vy_;
};
