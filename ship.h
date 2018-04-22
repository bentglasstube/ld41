#pragma once

#include "object.h"

#include <cmath>

#include "audio.h"
#include "graphics.h"
#include "spritemap.h"

#include "bullet.h"

class Ship : public Object {
  public:

    Ship();

    void thrust(double vx, double vy, bool boost);
    void weapon(Bullet::Type weapon);
    void get_shield();
    void get_fuel();
    Bullet* fire() const;
    void hurt(Audio& audio);

    void update(unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;
    inline bool dead() const override { return health_ <= 0; };
    inline int weapon_timer() const { return weapon_timer_; };
    inline int health() const { return health_; }
    inline int shield() const { return shield_; }
    inline int fuel() const { return fuel_; }
    inline bool boosting() const { return fuel_ > 0 && boosting_; }

  private:

    SpriteMap sprites_;
    double vx_, vy_;
    Bullet::Type weapon_;
    int weapon_timer_;
    int health_, shield_, fuel_;
    bool boosting_;

    static constexpr double kSpeed = 0.08;
};
