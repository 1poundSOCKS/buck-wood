#ifndef _play_screen_
#define _play_screen_

#define UNICODE

#include <memory>
#include <list>

#include "type_defs.h"
#include "system_timer.h"
#include "d2d_frame.h"
#include "control_state.h"
#include "sound_buffers.h"
#include "game_objects.h"

struct play_screen_state
{
  play_screen_state(const system_timer& timer, const game_level_data_index_ptr& gameLevelDataIndex);

  bool returnToMenu = false;
  
  enum STATE { state_playing, state_paused, state_level_complete, state_game_complete, state_player_dead };
  STATE state = state_playing;

  game_level_data_index_ptr gameLevelDataIndex;
  game_level_data_index::iterator currentLevelDataIterator;
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

using play_screen_state_ptr = std::unique_ptr<play_screen_state>;

void RenderFrame(const d2d_frame& frame, play_screen_state& playState);
void UpdateState(play_screen_state& playState, const control_state& controlState, const system_timer& timer);
void UpdateSound(const sound_buffers& soundBuffers, const play_screen_state& playState);

#endif
