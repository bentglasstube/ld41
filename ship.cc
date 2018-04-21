#include "ship.h"

#include "util.h"

Ship::Ship() : Object(92, 224), sprites_("ships.png", 4, 16, 16) {}

void Ship::thrust(double vx, double vy) {
  vx_ = vx;
  vy_ = vy;
}

void Ship::update(unsigned int elapsed) {
  x_ = Util::clamp(x_ + elapsed * vx_ * kSpeed, 8.0, 176.0);
  y_ = Util::clamp(y_ + elapsed * vy_ * kSpeed, 8.0, 232.0);
}

void Ship::draw(Graphics& graphics) const {
  const int n = vx_ < 0 ? 1 : vx_ > 0 ? 2 : 0;
  sprites_.draw(graphics, n, x_ - 8, y_ - 8);
}
