#pragma once

#include "object.h"

#include <unordered_map>

#include "graphics.h"
#include "spritemap.h"

class Bullet : public Object {
  public:
    enum class Type { Bullet, Laser };

    Bullet(double x, double y, Type type_);

    void update(unsigned int elapsed);
    void draw(Graphics& graphics) const;

    inline bool dead() const { return y_ < 0; };
    inline void kill() { y_ = -999; };
    inline Type type() const { return type_; };
    inline std::string audio_sample() const { return kSample.at(type_); };

  private:

    SpriteMap sprites_;
    Type type_;
    double vy_;
    int timer_;

    int sprite_index() const;

    static const std::unordered_map<Type, double> kSpeed;
    static const std::unordered_map<Type, std::string> kSample;
};
