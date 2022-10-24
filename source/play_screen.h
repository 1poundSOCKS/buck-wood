#ifndef _play_screen_
#define _play_screen_

#define UNICODE

#include <memory>
#include <list>

#include "type_defs.h"
#include "system_timer.h"
#include "game_level.h"
#include "d2d_frame.h"
#include "control_state.h"
#include "sound_buffers.h"

using game_data = std::list<game_level_data_ptr>;
using game_data_ptr = std::shared_ptr<game_data>;

struct play_state
{
  play_state(const system_timer& timer, const game_data_ptr& gameDataPtr);

  static const float gameSpeedMultiplier;

  bool returnToMenu = false;
  
  enum STATE { state_playing, state_level_complete, state_game_complete, state_player_dead };
  STATE state = state_playing;

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
  int64_t pauseTickCount = 0;
  float pauseTimeInSeconds = 0;
  float levelTimeRemaining = 0;
  float levelMouseX = 0, levelMouseY = 0;
  bool playerShot = false, targetShot = false;
};

using play_state_ptr = std::unique_ptr<play_state>;

void RenderFrame(const d2d_frame& frame, play_state& playState);
void UpdateState(play_state& playState, const control_state& controlState, const system_timer& timer);
void UpdateSound(const sound_buffers& soundBuffers, const play_state& playState);

#endif
