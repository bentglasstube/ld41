#pragma once

#include "object.h"

#include "graphics.h"
#include "spritemap.h"

class Bullet : public Object {
  public:
    Bullet(double x, double y);

    void update(unsigned int elapsed);
    void draw(Graphics& graphics) const;

    inline bool dead() const { return y_ < 0; };
    inline void kill() { y_ = -999; };

  private:

    SpriteMap sprites_;
    double vy_;
};
