#ifndef _level_edit_screen_
#define _level_edit_screen_

#include <memory>
#include <list>
#include "d2d_frame.h"
#include "control_state.h"
#include "system_timer.h"
#include "sound_buffers.h"
#include "perf_data.h"
#include "game_objects.h"
#include "diagnostics.h"

struct level_edit_screen_state
{
  level_edit_screen_state(const game_level_data_index_ptr& gameLevelDataIndex);

  bool returnToMenu = false;
  game_level_data_index_ptr gameLevelDataIndex;
  game_level_data_index::iterator currentLevelDataIterator;
  game_level_ptr currentLevel;
};

using level_edit_screen_state_ptr = std::unique_ptr<level_edit_screen_state>;

void RenderFrame(const d2d_frame& frame, level_edit_screen_state& state);
void UpdateState(level_edit_screen_state& screenState, const control_state& controlState, const system_timer& timer);
void UpdateSound(const sound_buffers& soundBuffers, const level_edit_screen_state& state);
void FormatDiagnostics(diagnostics_data& diagnosticsData, const level_edit_screen_state& screenState, const control_state& controlState, const perf_data& perfData, const system_timer& timer);

#endif
