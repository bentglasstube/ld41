#pragma once

#include "object.h"

#include "graphics.h"
#include "spritemap.h"

class Powerup : public Object {
  public:

    enum class Type { Up, Right, Down, Left, K, N, O, B };

    Powerup();

    void update(unsigned int elapased) override;
    void draw(Graphics& graphics) const override;
    inline bool dead() const override { return y_ > 250; };

    inline Type type() const { return type_; };
    void kill();
    bool rotate();

  private:

    SpriteMap sprites_;
    double vy_;
    Type type_;
};
