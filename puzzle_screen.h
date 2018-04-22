#pragma once

#include <array>
#include <random>
#include <vector>

#include "parallax_backdrop.h"
#include "screen.h"
#include "spritemap.h"
#include "text.h"
#include "title_screen.h"

#include "bullet.h"
#include "enemy.h"
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
    void reset(Puzzle::Difficulty diff);

  private:

    enum class GameState { Playing, Paused, Victory, Defeat };

    SpriteMap gui_, panel_;
    ParallaxBackdrop bg_;
    Text text_;
    std::mt19937 rand_;

    GameState state_;
    Puzzle puzzle_;
    Ship player_;

    std::vector<Powerup> powerups_;
    std::vector<Bullet*> bullets_;
    std::vector<Explosion> explosions_;
    std::vector<Enemy> enemies_;

    int timer_, state_timer_, enemy_timer_;
    int bgoffset_, choice_;

    bool collision(const Object& a, const Object& b, double r) const;

    void pause(Audio& audio);
    void resume(Audio& audio);
    void win(Audio& audio);
    void lose(Audio& audio);

    bool handle_menu(const Input& input, Audio& audio);
};
