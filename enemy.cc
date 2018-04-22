#include "enemy.h"

#include <cmath>

Enemy::Enemy(double x, double y, Type type) :
  Object(x, y), sprites_("enemies.png", 4, 16, 16),
  type_(type), timer_(0)
{
  switch (type) {
    case Type::Invader:
      x_ = std::round(x_ / 16) * 16 - 4;
      y_ = -10;
      vx_ = 0;
      vy_ = 0.5;
      break;

    case Type::Shroom:
      x_ = 92 + 102 * (x_ > 92 ? 1 : -1);
      y_ = std::round(y_ / 16) * 16 - 8;
      vx_ = 0.7 * (x_ > 92 ? -1 : 1);
      vy_ = 0;
      break;

    case Type::Crab:
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
  sprites_.draw(graphics, sprite_index(), x_ - 8, y_ - 8);
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
