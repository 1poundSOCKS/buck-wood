#ifndef _level_state_
#define _level_state_

#include "game_level_data.h"
#include "screen_input_state.h"
#include "timers.h"
#include "screen_render_data.h"

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

struct player_ship_point_data
{
  std::vector<game_point> points;
  std::vector<game_point> transformedPoints;
  std::vector<game_point> thrusterPoints;
};

struct bullet
{
  bullet();

  bool free = true;
  float startX = 0, startY = 0;
  float xPos = 0, yPos = 0;
  float xVelocity = 0, yVelocity = 0;
  float angle = 0;
  float range = 0;
};

struct object_state
{
  object_state(const game_level_object_data& data);

  const game_level_object_data& data;
  std::vector<game_line> shape;
};

struct target_state
{
  target_state(const game_point& position);

  const game_point& position;
  std::vector<game_point> points;
  bool activated = false;
  std::vector<game_line> shape;
};

struct level_control_state
{
  bool thrust = false;
  bool shoot = false;
  render_target_mouse_data renderTargetMouseData;
};

struct level_state
{
  level_state(const game_level_data& levelData, int64_t counterFrequency);

  const game_level_data& levelData;
  const int64_t counterFrequency;
  const int64_t shotTimerInterval;

  int64_t currentTimerCount = 0;
  int64_t previousTimerCount = 0;
  int64_t shotCount = 0;

  player_ship player;

  float mouseX = 0;
  float mouseY = 0;
  bool playerShot = false;
  bool targetShot = false;

  std::vector<game_line> boundaryLines;
  std::vector<object_state> objects;
  std::vector<target_state> targets;
  std::vector<bullet> bullets;

  player_ship_point_data playerShipPointData;
};

void UpdateLevelState(level_state& levelState, const level_control_state& controlState, int64_t counterValue);

bool LevelIsComplete(const level_state& levelState);

D2D1::Matrix3x2F CreateViewTransform(const level_state& levelState, const D2D1_SIZE_F& renderTargetSize, float renderScale = 1.0);

#endif
