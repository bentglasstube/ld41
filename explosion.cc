#include "explosion.h"

Explosion::Explosion(double x, double y) :
  Object(x, y),
  sprites_("ships.png", 4, 16, 16),
  timer_(0) {}

void Explosion::update(unsigned int elapsed) {
  timer_ += elapsed;
}

void Explosion::draw(Graphics& graphics) const {
  const int n = 8 + timer_ / (kFrameTime / 4);
  if (n > 11) return;
  sprites_.draw(graphics, n, x_ - 8, y_ - 8);
}
