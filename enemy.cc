#include "enemy.h"

Enemy::Enemy(double x, double y, Type type) :
  Object(x, y), sprites_("enemies.png", 4, 16, 16),
  type_(type), vx_(0), vy_(0) {}

void Enemy::ai(const Object& target) {
  if (target.x() < x_) vx_ = -1;
  if (target.x() > x_) vx_ = 1;

  if (target.y() < y_) vy_ = -1;
  if (target.y() > y_) vy_ = 1;
}

void Enemy::update(unsigned int elapsed) {
  timer_ += elapsed;

  x_ += vx_;
  y_ += vy_;
}

void Enemy::draw(Graphics& graphics) const {
  sprites_.draw(graphics, sprite_index(), x_ - 8, y_ - 8);
}

int Enemy::sprite_index() const {
  return static_cast<int>(type_) * 4 + (timer_ / 125) % 4;
}
