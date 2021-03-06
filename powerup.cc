#include "powerup.h"

#include <random>

#include "util.h"

Powerup::Powerup(double x, double y) : Object(x, y), sprites_("powerups.png", 4, 16, 16) {
  std::mt19937 rand;
  rand.seed(Util::random_seed());

  std::uniform_real_distribution<double> vydist(0.04, 0.09);
  vy_ = vydist(rand);

  std::uniform_int_distribution<int> tdist(0, 100);
  const int r = tdist(rand);
  if (r < 50) {
    std::uniform_int_distribution<int> ddist(0, 3);
    type_ = static_cast<Type>(ddist(rand));
  } else if (r < 75) {
    type_ = Type::K;
  } else {
    std::uniform_int_distribution<int> odist(5, 7);
    type_ = static_cast<Type>(odist(rand));
  }
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

bool Powerup::rotate() {
  const int n = static_cast<int>(type_);
  if (n < 4) {
    type_ = static_cast<Type>((n + 1) % 4);
    return true;
  } else {
    return false;
  }
}
