#ifndef _level_edit_screen_
#define _level_edit_screen_

#include <memory>
#include <list>
#include "d2d_frame.h"
#include "control_state.h"
#include "system_timer.h"
#include "sound_buffers.h"
#include "perf_data.h"

struct level_edit_screen_state
{
  bool returnToMenu = false;
};

using level_edit_screen_state_ptr = std::unique_ptr<level_edit_screen_state>;

void RenderFrame(const d2d_frame& frame, level_edit_screen_state& state);
void UpdateState(level_edit_screen_state& screenState, const control_state& controlState, const system_timer& timer);
void UpdateSound(const sound_buffers& soundBuffers, const level_edit_screen_state& state);
void FormatDiagnostics(std::list<std::wstring>& diagnostics, const level_edit_screen_state& screenState, const control_state& controlState, const perf_data& perfData, const system_timer& timer);

#endif
