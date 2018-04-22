#pragma once

#include "object.h"

#include <cmath>

#include "graphics.h"
#include "spritemap.h"

#include "bullet.h"

class Ship : public Object {
  public:

    Ship();

    void thrust(double vx, double vy = 0);
    void weapon(Bullet::Type weapon);
    Bullet* fire() const;
    void hurt();

    void update(unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;
    inline bool dead() const override { return health_ <= 0; };
    inline int weapon_timer() const { return weapon_timer_; };
    inline int health() const { return health_; }

  private:

    SpriteMap sprites_;
    double vx_, vy_;
    Bullet::Type weapon_;
    int weapon_timer_, health_;

    static constexpr double kSpeed = 0.05;
};
