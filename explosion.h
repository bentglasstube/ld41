#pragma once

#include "object.h"

#include "graphics.h"
#include "spritemap.h"

class Explosion : public Object {
  public:

    Explosion(double x, double y);

    void update(unsigned int elapsed);
    void draw(Graphics& graphics) const;

    inline bool dead() const { return timer_ > kFrameTime * 4; }

  private:

    SpriteMap sprites_;
    int timer_;

    static constexpr int kFrameTime = 200;
};
