#include "level_edit_screen.h"

void RenderFrame(const d2d_frame& frame, level_edit_screen_state& state)
{
  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
}

void UpdateState(level_edit_screen_state& screenState, const control_state& controlState, const system_timer& timer)
{
  screenState.returnToMenu = false;
  
  if( controlState.quitPress ) screenState.returnToMenu = true;
}

void UpdateSound(const sound_buffers& soundBuffers, const level_edit_screen_state& state)
{
}

void FormatDiagnostics(std::list<std::wstring>& diagnostics, const level_edit_screen_state& screenState, const control_state& controlState, const perf_data& perfData, const system_timer& timer)
{
}
