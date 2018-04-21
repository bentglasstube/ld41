#include "bullet.h"

Bullet::Bullet(double x, double y) : sprites_("ships.png", 4, 16, 16), x_(x), y_(y), vy_(-0.2) {}

void Bullet::update(unsigned int elapsed) {
  y_ += vy_ * elapsed;
}

void Bullet::draw(Graphics& graphics) const {
  sprites_.draw(graphics, 4, x_ - 8, y_ - 8);
}
