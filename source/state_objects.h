#ifndef _state_objects_
#define _state_objects_

#include "game_math.h"
#include "game_level.h"
#include "control_state.h"
#include "system_timer.h"
#include "game_events.h"
#include "level_editor.h"

using game_data = std::list<game_level_data_ptr>;
using game_data_ptr = std::shared_ptr<game_data>;

struct play_state
{
  play_state(const system_timer& timer, const game_data_ptr& gameDataPtr);

  enum STATE { state_playing, state_levelend, state_complete };
  STATE state = state_playing;

  enum PLAYER_STATE { player_alive, player_dead };
  PLAYER_STATE playerState = player_alive;

  enum LEVEL_STATE { level_incomplete, level_complete };
  LEVEL_STATE levelState = level_incomplete;

  game_data_ptr gameData;
  game_data::iterator currentLevelDataIterator;
  game_level_ptr currentLevel;

  std::unique_ptr<player_ship> player;
  std::list<std::unique_ptr<bullet>> bullets;

  int64_t totalTicks = 0;
  int64_t ticksPerSecond = 0;
  int64_t levelTimerStart = 0;
  int64_t levelTimerStop = 0;
  int64_t lastShotTicks = 0;
  int64_t levelEndTickCount = 0;

  float levelTimeRemaining = 0;

  float levelMouseX = 0, levelMouseY = 0;

  static const float gameSpeedMultiplier;
};

using play_state_ptr = std::unique_ptr<play_state>;

struct game_state
{
  game_state();

  bool starting = true;
  bool running = true;
};

using game_state_ptr = std::unique_ptr<game_state>;

#endif
