#pragma once

#include "object.h"

#include <unordered_map>

#include "graphics.h"
#include "spritemap.h"
#include "util.h"

class Bullet : public Object {
  public:
    enum class Type { Bullet, Laser };

    Bullet(double x, double y, Type type);

    void update(unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;
    inline bool dead() const override { return y_ < 0; };
    inline void kill() override { y_ = -999; };

    inline Type type() const { return type_; };
    inline std::string audio_sample() const { return kSample.at(type_); };

  private:

    SpriteMap sprites_;
    Type type_;
    double vy_;
    int timer_;

    int sprite_index() const;

    static const std::unordered_map<Type, double, Util::CastHash<Type>> kSpeed;
    static const std::unordered_map<Type, std::string, Util::CastHash<Type>> kSample;
};
