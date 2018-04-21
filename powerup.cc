#include "powerup.h"

#include <random>

#include "util.h"

Powerup::Powerup() : sprites_("powerups.png", 4, 16, 16), x_(0), y_(0), vy_(-10) {
  vy_ = 0.07;

  std::mt19937 rand;
  rand.seed(Util::random_seed());

  std::uniform_real_distribution<double> xdist(8, 176);
  x_ = xdist(rand);

  std::uniform_real_distribution<double> vydist(0.04, 0.09);
  vy_ = vydist(rand);

  std::uniform_int_distribution<int> tdist(0, 3);
  type_ = static_cast<Type>(tdist(rand));
}

void Powerup::update(unsigned int elapased) {
  y_ += vy_ * elapased;
}

void Powerup::draw(Graphics& graphics) const {
  sprites_.draw(graphics, static_cast<int>(type_), x_ - 8, y_ - 8);
}

void Powerup::kill() {
  y_ = 999;
}

bool Powerup::touching(double x, double y, double r) const {
  const double dx = x - x_;
  const double dy = y - y_;
  return dx * dx + dy * dy < (8 + r) * (8 + r);
}
