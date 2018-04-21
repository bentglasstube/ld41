#pragma once

#include <array>
#include <random>
#include <vector>

#include "parallax_backdrop.h"
#include "screen.h"
#include "spritemap.h"
#include "text.h"

#include "bullet.h"
#include "explosion.h"
#include "powerup.h"
#include "puzzle.h"
#include "ship.h"

class PuzzleScreen : public Screen {
  public:
    PuzzleScreen();

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;

  private:

    SpriteMap gui_;
    ParallaxBackdrop bg_;
    Text text_;
    std::mt19937 rand_;

    Puzzle puzzle_;
    Ship player_;
    std::vector<Powerup> powerups_;
    std::vector<Bullet> bullets_;
    std::vector<Explosion> explosions_;

    int timer_;

    void reset();
    bool collision(const Object& a, const Object& b, double r) const;
};
