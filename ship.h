#pragma once

#include "object.h"

#include "graphics.h"
#include "spritemap.h"

#include "bullet.h"

class Ship : public Object {
  public:

    Ship();

    void thrust(double vx, double vy = 0);
    void weapon(Bullet::Type weapon);
    Object* fire() const;

    void update(unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;
    inline bool dead() const override { return false; };

  private:

    SpriteMap sprites_;
    double vx_, vy_;
    Bullet::Type weapon_;

    static constexpr double kSpeed = 0.05;
};
