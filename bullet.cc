#include "bullet.h"

Bullet::Bullet(double x, double y, Bullet::Type type) :
  Object(x, y),
  sprites_("ships.png", 4, 16, 16),
  type_(type)
{
  vy_ = kSpeedForType.at(type);
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

const std::unordered_map<Bullet::Type, double> Bullet::kSpeedForType = {
  { Type::Bullet, -0.2 },
  { Type::Laser, -0.35 },
};

