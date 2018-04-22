#include "bar.h"

Bar::Bar(Bar::Color color, int max, int width) :
  sprites_("bars.png", 9, 2, 8),
  color_(color), max_(max), width_(width) {}

void Bar::draw(Graphics& graphics, int x, int y, int value) const {
  const int c = 9 * static_cast<int>(color_);
  const int f = width_ * value / max_;

  sprites_.draw(graphics, c + (f == 0 ? 0 : f == 1 ? 1 : 2), x, y);

  for (int i = 2; i < width_ - 2; i += 2) {
    sprites_.draw(graphics, c + (f < i ? 5 : f == i ? 4 : 3), x + i, y);
  }
  sprites_.draw(graphics, c + (f == width_ ? 8 : f == width_ - 1 ? 7 : 6), x + width_ - 2, y);
}
