#include "enemy.h"

#include <cmath>
#include <random>

Enemy::Enemy(double x, double y, Type type, const Object& target ) :
  Object(x, y), sprites_("enemies.png", 4, 16, 16),
  type_(type), timer_(0)
{
  std::mt19937 rand;
  rand.seed(Util::random_seed());

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
      {
        std::uniform_real_distribution<double> vxdist(-0.2, 0.2);

        x_ = std::round(x_ / 16) * 16 - 4;
        y_ = -8;
        vx_ = vxdist(rand);
        vy_ = 0.5;
      }
      break;

    case Type::Elephant:
      y_ = -8;
      vx_ = 0;
      vy_ = 0.2;
      break;

    case Type::GhostCrab:
      y_ = -8;
      move_toward(92, 60, 1.5);
      break;

    case Type::Eyeball:
      move_toward(target.x(), target.y(), 0.8);
      break;

    case Type::Fireball:
      move_toward(target.x(), target.y(), 1.25);
      break;

    default:
      break;
  }

  shot_timer_ = kShotInterval.at(type_);
}

bool Enemy::fire() {
  const int ival = kShotInterval.at(type_);
  if (shot_timer_ < 0 && ival > 0) {
    shot_timer_ += ival;
    return true;
  }

  return false;
}

void Enemy::update(unsigned int elapsed) {
  timer_ += elapsed;

  x_ += vx_;
  y_ += vy_;

  if (y_ > 0) shot_timer_ -= elapsed;

  if (type_ == Type::Elephant) {
    if (y_ > 64) {
      y_ = 64;
      vy_ = 0;
    }
  }
}

void Enemy::draw(Graphics& graphics) const {
  sprites_.draw(graphics, sprite_index(), x() - 8, y() - 8);
}

double Enemy::x() const {
  switch (type_) {
    case Type::Crab:
      return x_ + 32 * std::sin(timer_ / 500.0);

    case Type::Elephant:
      return 92 + 64 * std::sin(timer_ / 500.0) + 16 * std::cos(timer_ / 350.0);

    case Type::GhostCrab:
      return x_ + 80 * Util::clamp(timer_ / 2500.0, 0.0, 1.0) * std::cos(timer_ / 400.0);

    default:
      return x_;
  }
}

double Enemy::y() const {
  switch (type_) {
    case Type::Jelly:
      return y_ + 16 * std::sin(timer_ / 250.0);

    case Type::GhostCrab:
      return y_ + 80 * Util::clamp(timer_ / 2500.0, 0.0, 1.0) * std::sin(timer_ / 400.0);

    default:
      return y_;
  }
}

int Enemy::sprite_index() const {
  return static_cast<int>(type_) * 4 + ((timer_ / 125) % 4);
}

void Enemy::move_toward(double tx, double ty, double speed) {
  const double a = std::atan2(ty - y(), tx - x());
  vx_ = std::cos(a) * speed;
  vy_ = std::sin(a) * speed;
}

const std::unordered_map<Enemy::Type, int, Util::CastHash<Enemy::Type>> Enemy::kShotInterval = {
  { Enemy::Type::Invader,   100000 },
  { Enemy::Type::Shroom,    100000 },
  { Enemy::Type::Crab,        5000 },
  { Enemy::Type::Jelly,       5000 },
  { Enemy::Type::Elephant,    2500 },
  { Enemy::Type::GhostCrab,    500 },
  { Enemy::Type::Eyeball,        0 },
  { Enemy::Type::Fireball,       0 },
};
