#ifndef _play_screen_
#define _play_screen_

#include "framework.h"
#include "control_state.h"
#include "game_objects.h"
#include "diagnostics.h"
#include "play_screen_control_state.h"
#include "level_state.h"
#include "screen_render.h"

struct play_screen_state
{
  play_screen_state(const system_timer& timer, game_level_data_index::const_iterator currentLevelDataIterator, game_level_data_index::const_iterator endLevelDataIterator);

  const system_timer& systemTimer;

  enum STATE { state_playing, state_paused, state_level_complete, state_game_complete, state_player_dead };
  STATE state = state_playing;
  bool returnToMenu = false;

  game_level_data_index::const_iterator currentLevelDataIterator;
  game_level_data_index::const_iterator endLevelDataIterator;

  render_target_mouse_data renderTargetMouseData;

  std::unique_ptr<stopwatch> levelTimer;
  std::unique_ptr<stopwatch> pauseTimer;
  std::unique_ptr<level_state> levelState;
  std::vector<float> levelTimes;
};

void UpdateScreenState(play_screen_state& screenState, const play_screen_control_state_reader& controlStateReader, const system_timer& timer);

bool ContinueRunning(const play_screen_state& screenState);

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const play_screen_state& screenState, const play_screen_control_state& controlState);

#endif
