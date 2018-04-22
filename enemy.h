#pragma once

#include "object.h"

#include <unordered_map>

#include "graphics.h"
#include "spritemap.h"
#include "util.h"

class Enemy : public Object {

  public:
    enum class Type { Invader, Shroom, Crab, Jelly, Elephant, GhostCrab, Eyeball, Fireball };

    Enemy(double x, double y, Type type, const Object& target);

    bool fire();

    void update(unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;
    inline bool dead() const override { return x() < -10 || x() > 194 || y() > 266; };
    inline void kill() override { y_ = 999; };

    inline Type type() const { return type_; };

    double x() const override;
    double y() const override;

  private:

    SpriteMap sprites_;
    Type type_;
    double vx_, vy_;
    int timer_, shot_timer_;

    int sprite_index() const;
    void move_toward(double x, double y, double speed);

    static const std::unordered_map<Type, int, Util::CastHash<Type>> kShotInterval;
};


