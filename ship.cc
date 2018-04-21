#include "ship.h"

Ship::Ship() : sprites_("ships.png", 4, 16, 16), x_(92.0), vx_(0.0) {}

void Ship::thrust(double v) {
  vx_ = v;
}

void Ship::update(unsigned int elapsed) {
  x_ += elapsed * vx_ * kSpeed;
  if (x_ < 8) {
    x_ = 8;
    vx_ = 0;
  } else if (x_ > 176) {
    x_ = 176;
    vx_ = 0;
  }
}

void Ship::draw(Graphics& graphics, int y) const {
  const int n = vx_ < 0 ? 1 : vx_ > 0 ? 2 : 0;
  sprites_.draw(graphics, n, x_ - 8, y);
}
