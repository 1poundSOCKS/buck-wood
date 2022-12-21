#ifndef _play_screen_state_
#define _play_screen_state_

#include "framework.h"
#include "game_objects.h"
#include "level_state.h"
#include "timers.h"
#include "screen_sound_data.h"

struct play_screen_state
{
  play_screen_state(
    game_level_data_index::const_iterator currentLevelDataIterator, 
    game_level_data_index::const_iterator endLevelDataIterator,
    const bespoke_render_data& renderData,
    const bespoke_sound_data& bespokeSoundData
  );

  const bespoke_render_data renderData;
  const bespoke_sound_data soundData;

  performance_counter::data timer = { 0, 0, 0 };
  int64_t pauseStart = 0;
  int64_t pauseTotal = 0;
  int64_t levelStart = 0;
  int64_t transitionEnd = 0;

  enum mode_type { playing, paused, level_complete, game_complete, player_dead };
  mode_type mode = playing;
  bool continueRunning = true;

  game_level_data_index::const_iterator currentLevelDataIterator;
  game_level_data_index::const_iterator endLevelDataIterator;

  render_target_mouse_data renderTargetMouseData;

  std::unique_ptr<level_state> levelState;
  std::vector<float> levelTimes;
};

#endif
