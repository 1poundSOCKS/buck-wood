#ifndef _level_state_
#define _level_state_

#include "game_level_data.h"
#include "control_state.h"

struct player_ship
{
  player_ship();

  enum player_state { state_alive, state_dead };

  player_state state = state_alive;
  float xPos, yPos;
  float xVelocity, yVelocity;
  float angle;
  bool thrusterOn = false;
};

struct bullet
{
  bullet(float x, float y, float range);

  float startX, startY;
  float xPos, yPos;
  float xVelocity, yVelocity;
  float angle;
  float range;
  bool outsideLevel;
};

struct target_state
{
  target_state(const game_point& position);

  const game_point& position;
  std::vector<game_point> points;
  bool activated = false;
};

struct level_control_state
{
  bool thrust = false, shoot = false;
  render_target_mouse_data renderTargetMouseData;
};

struct level_state
{
  level_state(const game_level_data& levelData, const system_timer& systemTimer);

  const game_level_data& levelData;
  const system_timer& systemTimer;
  player_ship player;
  std::list<std::unique_ptr<bullet>> bullets;
  std::vector<target_state> targets;
  float mouseX = 0, mouseY = 0;
  stopwatch shotTimer;
  bool playerShot = false, targetShot = false;
};

void RefreshControlState(level_control_state& controlState, const control_state& baseControlState);
void UpdateState(level_state& levelState, const level_control_state& controlState, const system_timer& timer);
bool LevelIsComplete(const level_state& levelState);
void CreateRenderLines(const level_state& levelState, std::back_insert_iterator<std::vector<render_line>> renderLines);
D2D1::Matrix3x2F CreateViewTransform(const level_state& levelState, const D2D1_SIZE_F& renderTargetSize);

#endif
