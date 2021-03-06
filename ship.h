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
    void weapon();
    void get_shield();
    void get_fuel();
    Bullet* fire();
    void hurt(Audio& audio);

    void update(unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;
    inline bool dead() const override { return health_ <= 0; };
    inline int weapon_shots() const { return weapon_shots_; };
    inline int health() const { return health_; }
    inline int shield() const { return shield_; }
    inline int fuel() const { return fuel_; }
    inline bool boosting() const { return fuel_ > 0 && boosting_; }

  private:

    SpriteMap sprites_;
    double vx_, vy_;
    int weapon_shots_;
    int health_, shield_, fuel_;
    bool boosting_;

    static constexpr double kSpeed = 0.08;
};
