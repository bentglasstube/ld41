#pragma once

#include "graphics.h"
#include "spritemap.h"

class Bar {
  public:
    enum class Color { Blue, Red, Green, Yellow };

    Bar(Color color, int max, int width);
    void draw(Graphics& graphics, int x, int y, int value) const;

  private:

    SpriteMap sprites_;
    Color color_;
    int max_, width_;
};
