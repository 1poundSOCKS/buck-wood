#ifndef _game_state_
#define _game_state_

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
  play_state();

  enum STATE { state_playing, state_levelend, state_complete };
  STATE state = state_playing;

  enum PLAYER_STATE { player_alive, player_dead };

  enum LEVEL_STATE { level_incomplete, level_complete };
  LEVEL_STATE levelState = level_incomplete;

  game_data_ptr gameData;
  game_data::iterator currentLevelDataIterator;
  game_level_ptr currentLevel;

  PLAYER_STATE playerState = player_alive;
  std::unique_ptr<player_ship> player;
  std::list<std::unique_ptr<bullet>> bullets;

  int64_t totalTicks = 0;
  int64_t ticksPerSecond = 0;
  int64_t levelTimerStart = 0;
  int64_t levelTimerStop = 0;
  int64_t lastShotTicks = 0;
  int64_t levelEndTickCount = 0;

  float levelMouseX = 0, levelMouseY = 0;

  static const float gameSpeedMultiplier;
};

using play_state_ptr = std::unique_ptr<play_state>;

struct game_state
{
  game_state();

  enum SCREEN { screen_title, screen_play, screen_level_editor };
  SCREEN screen = screen_title;

  float renderTargetMouseX = 0, renderTargetMouseY = 0;
  bool starting = true;
  bool running = true;
  play_state_ptr playState;
  level_editor_state_ptr levelEditorState;
  game_data_ptr gameData;
};

using game_state_ptr = std::unique_ptr<game_state>;

#endif
