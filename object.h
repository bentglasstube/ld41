#pragma once

#include "graphics.h"

class Object {
  public:
    Object(double x, double y);

    virtual void update(unsigned int elapsed) = 0;
    virtual void draw(Graphics& graphics) const = 0;
    virtual bool dead() const = 0;
    virtual void kill() {};

    virtual inline double x() const { return x_; }
    virtual inline double y() const { return y_; }

  protected:

    double x_, y_;

};
