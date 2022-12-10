#ifndef _play_screen_state_
#define _play_screen_state_

#include "framework.h"
#include "game_objects.h"
#include "level_state.h"

struct play_screen_state
{
  play_screen_state(
    game_level_data_index::const_iterator currentLevelDataIterator, 
    game_level_data_index::const_iterator endLevelDataIterator
  );

  performance_counter::data timer = { 0, 0, 0 };
  int64_t pauseStartCount = 0;
  int64_t pauseTotalCount = 0;
  int64_t levelStartCount = 0;
  int64_t transitionEndCount = 0;

  enum STATE { state_playing, state_paused, state_level_complete, state_game_complete, state_player_dead };
  STATE state = state_playing;
  bool returnToMenu = false;

  game_level_data_index::const_iterator currentLevelDataIterator;
  game_level_data_index::const_iterator endLevelDataIterator;

  render_target_mouse_data renderTargetMouseData;

  std::unique_ptr<level_state> levelState;
  std::vector<float> levelTimes;
};

#endif
