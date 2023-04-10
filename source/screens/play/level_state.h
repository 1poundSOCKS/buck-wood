#ifndef _level_state_
#define _level_state_

#include "game_level_data.h"
#include "player_ship.h"
#include "bullet.h"
#include "screen_input_state.h"
#include "screen_render_data.h"
#include "level_control_state.h"
#include "area_grid.h"
#include "explosion.h"
#include "level_target.h"
#include "level_island.h"
#include "solid_object.h"
#include "play_event.h"
#include "starfield.h"

struct level_state
{
  level_state(const game_level_data& levelData, int64_t counterFrequency, const screen_render_data& renderData);

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

  game_rect viewRect;
  
  bool playerShot = false;
  bool targetShot = false;

  std::shared_ptr<player_ship::data_type> playerData;  
  std::list<solid_object> solidObjects;
  std::shared_ptr<player_control_state> controlState;
  screen_render_brush_selector brushes;
};

void UpdateLevelState(level_state& levelState, const level_control_state& controlState, int64_t counterValue);
bool LevelIsComplete(const level_state& levelState);
bool PlayerIsDead(const level_state& levelState);
D2D1::Matrix3x2F CreateViewTransform(const level_state& levelState, const D2D1_SIZE_F& renderTargetSize, float renderScale = 1.0);
bool LevelTimedOut(const level_state& levelState);
int64_t GetPlayTimeRemaining(const level_state& screenState);
float GetPlayTimeRemainingInSeconds(const level_state& screenState);

#endif
