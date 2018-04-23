#include "ship.h"

#include "util.h"

#include "bullet.h"

Ship::Ship() :
  Object(92, 224), sprites_("ships.png", 4, 16, 16),
  weapon_shots_(0), health_(16), shield_(0), fuel_(2500) {}

void Ship::thrust(double vx, double vy, bool boost) {
  vx_ = vx;
  vy_ = vy;
  boosting_ = boost;
}

void Ship::weapon() {
  weapon_shots_ = 25;
}

void Ship::get_shield() {
  shield_ = 30000;
}

void Ship::get_fuel() {
  fuel_ = std::min(fuel_ + 500, 5000);
}

Bullet* Ship::fire() {
  Bullet* b = new Bullet(x_, y_ - 4, weapon_shots_ > 0 ? Bullet::Type::Laser : Bullet::Type::Bullet);
  --weapon_shots_;
  return std::move(b);
}

void Ship::hurt(Audio& audio) {
  if (shield_ > 0) {
    shield_ -= 5000;
    audio.play_sample("shield.wav");
  } else {
    health_ -= 1;
    audio.play_sample(health_ > 0 ? "hurt.wav" : "dead.wav");
  }
}

void Ship::update(unsigned int elapsed) {
  const double s = kSpeed * (boosting() ? 2.5 : 1);
  x_ = Util::clamp(x_ + elapsed * vx_ * s, 8.0, 176.0);
  y_ = Util::clamp(y_ + elapsed * vy_ * s, 8.0, 232.0);

  if (boosting()) {
   fuel_ -= elapsed;
   if (fuel_ < 0) fuel_ = 0;
  }

  if (shield_ > 0) shield_ -= elapsed;
}

void Ship::draw(Graphics& graphics) const {
  if (!dead()) {
    const int n = vx_ < 0 ? 1 : vx_ > 0 ? 2 : 0;
    sprites_.draw(graphics, n, x_ - 8, y_ - 8);
    if (shield_ > 0) sprites_.draw(graphics, 3, x_ - 8, y_ - 8);
  }
}
