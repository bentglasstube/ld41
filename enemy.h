#pragma once

#include "object.h"

#include <unordered_map>

#include "graphics.h"
#include "spritemap.h"

class Enemy : public Object {

  public:
    enum class Type { Invader, Shroom, Crab, Jelly, Elephant, GhostCrab, Eyeball };

    Enemy(double x, double y, Type type);

    void ai(const Object& target);

    void update(unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;
    inline bool dead() const override { return y_ > 500; }
    inline void kill() override { y_ = 999; };

    inline Type type() const { return type_; };

  private:

    SpriteMap sprites_;
    Type type_;
    double vx_, vy_;
    int timer_;

    int sprite_index() const;
};
