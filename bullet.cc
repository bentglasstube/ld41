#include "bullet.h"

Bullet::Bullet(double x, double y, Bullet::Type type) :
  Object(x, y),
  sprites_("ships.png", 4, 16, 16),
  type_(type), timer_(0)
{
  vy_ = kSpeed.at(type);
}

void Bullet::update(unsigned int elapsed) {
  y_ += vy_ * elapsed;
  timer_ += elapsed;
}

void Bullet::draw(Graphics& graphics) const {
  sprites_.draw(graphics, sprite_index(), x_ - 8, y_ - 8);
}

int Bullet::sprite_index() const {
  switch (type_) {
    case Type::Bullet:
      return 4;

    case Type::Laser:
      return (timer_ / 100) % 3 + 5;

    default:
      return 3;
  }
}

const std::unordered_map<Bullet::Type, double, Util::CastHash<Bullet::Type>> Bullet::kSpeed = {
  { Type::Bullet, -0.2 },
  { Type::Laser, -0.35 },
};

const std::unordered_map<Bullet::Type, std::string, Util::CastHash<Bullet::Type>> Bullet::kSample = {
  { Type::Bullet, "bullet.wav" },
  { Type::Laser, "laser.wav" },
};

