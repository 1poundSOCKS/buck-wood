#ifndef _level_state_
#define _level_state_

#include "game_level_data.h"
#include "level_objects.h"
#include "screen_input_state.h"
#include "screen_render_data.h"
#include "level_geometry.h"

struct level_control_state
{
  bool thrust = false;
  bool shoot = false;
  render_target_mouse_data renderTargetMouseData;
};

struct level_state
{
  level_state(
    const game_level_data& levelData, 
    int64_t counterFrequency, 
    const screen_render_data& renderData
  );

  const game_level_data& levelData;
  const int64_t counterFrequency;
  int64_t levelTimeLimit;

  int64_t currentTimerCount = 0;
  int64_t previousTimerCount = 0;

  int64_t shotTimerInterval;
  int64_t lastShotTimerValue = 0;

  D2D1::Matrix3x2F viewTransform;
  D2D1::Matrix3x2F invertedViewTransform;
  
  float mouseX = 0;
  float mouseY = 0;
  
  bool playerShot = false;
  bool targetShot = false;

  player_ship player;
  std::vector<game_line> groundLines;
  std::vector<game_line> objectLines;
  std::vector<target_state> targets;
  std::vector<bullet> bullets;
  level_background_data backgroundData;
  std::vector<render_point> renderStars;
  level_ground_geometry groundGeometry;
  level_targets_geometry targetsGeometry;
};

void UpdateLevelState(level_state& levelState, const level_control_state& controlState, int64_t counterValue);
bool LevelIsComplete(const level_state& levelState);
bool PlayerIsDead(const level_state& levelState);
D2D1::Matrix3x2F CreateViewTransform(const level_state& levelState, const D2D1_SIZE_F& renderTargetSize, float renderScale = 1.0);
bool LevelTimedOut(const level_state& levelState);
int64_t GetPlayTimeRemaining(const level_state& screenState);
float GetPlayTimeRemainingInSeconds(const level_state& screenState);

#endif
