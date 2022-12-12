#ifndef _level_state_
#define _level_state_

#include "game_level_data.h"
#include "level_objects.h"
#include "screen_input_state.h"
#include "screen_render_data.h"

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
  int64_t levelTimeLimit;

  int64_t currentTimerCount = 0;
  int64_t previousTimerCount = 0;

  int64_t shotTimerInterval;
  int64_t lastShotTimerValue = 0;

  player_ship player;

  float mouseX = 0;
  float mouseY = 0;
  bool playerShot = false;
  bool targetShot = false;

  std::vector<game_line> theGround;
  std::vector<game_line> objectLines;

  std::vector<object_state> objectStates;
  std::vector<target_state> targets;
  std::vector<bullet> bullets;

  player_ship_point_data playerShipPointData;
};

void UpdateLevelState(level_state& levelState, const level_control_state& controlState, int64_t counterValue);
bool LevelIsComplete(const level_state& levelState);
D2D1::Matrix3x2F CreateViewTransform(const level_state& levelState, const D2D1_SIZE_F& renderTargetSize, float renderScale = 1.0);
int64_t GetPlayTimeRemaining(const level_state& screenState);
float GetPlayTimeRemainingInSeconds(const level_state& screenState);

#endif
