#include "enemy.h"

#include <cmath>

Enemy::Enemy(double x, double y, Type type) :
  Object(x, y), sprites_("enemies.png", 4, 16, 16),
  type_(type), timer_(0)
{
  switch (type) {
    case Type::Invader:
      x_ = std::round(x_ / 16) * 16 - 4;
      y_ = -8;
      vx_ = 0;
      vy_ = 0.5;
      break;

    case Type::Shroom:
      x_ = x_ > 92 ? -8 : 192;
      y_ = std::round(y_ / 16) * 16 - 8;
      vx_ = 0.7 * (x_ > 92 ? -1 : 1);
      vy_ = 0;
      break;

    case Type::Crab:
      x_ = std::round(x_ / 16) * 16 - 4;
      y_ = -8;
      vx_ = 0;
      vy_ = 0.25;

      break;

    case Type::Jelly:
      break;

    case Type::Elephant:
      break;

    case Type::GhostCrab:
      break;

    case Type::Eyeball:
      break;

  }
}

void Enemy::ai(const Object& target) {
  const int ival = kShotInterval.at(type_);
  if (shot_timer_ < 0 && ival > 0) {
    // todo fire shot
    shot_timer_ += ival;
  }
}

void Enemy::update(unsigned int elapsed) {
  timer_ += elapsed;

  x_ += vx_;
  y_ += vy_;

  if (y_ > 0) shot_timer_ -= elapsed;
}

void Enemy::draw(Graphics& graphics) const {
  sprites_.draw(graphics, sprite_index(), x() - 8, y() - 8);
}

double Enemy::x() const {
  switch (type_) {
    case Type::Crab:
      return x_ + 32 * std::sin(timer_ / 500.0f);

    default:
      return x_;
  }
}

double Enemy::y() const {
  switch (type_) {
    default:
      return y_;
  }
}

int Enemy::sprite_index() const {
  return static_cast<int>(type_) * 4 + ((timer_ / 125) % 4);
}

const std::unordered_map<Enemy::Type, int, Util::CastHash<Enemy::Type>> Enemy::kShotInterval = {
  { Enemy::Type::Invader,   0 },
  { Enemy::Type::Shroom,    0 },
  { Enemy::Type::Crab,      0 },
  { Enemy::Type::Jelly,     0 },
  { Enemy::Type::Elephant,  0 },
  { Enemy::Type::GhostCrab, 0 },
  { Enemy::Type::Eyeball,   0 },
};
