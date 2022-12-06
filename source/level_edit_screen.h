#ifndef _level_edit_screen_
#define _level_edit_screen_

#include "screens/level_edit/level_edit_screen_state.h"
#include "sound_buffer_selector.h"
#include "screen_render_data.h"
#include "timers.h"

void UpdateScreenState
(
  level_edit_screen_state& screenState, 
  const screen_input_state& inputState, 
  const system_timer& timer
);

void RenderFrame
(
  ID2D1RenderTarget* renderTarget, 
  screen_render_brush_selector renderBrushSelector, 
  screen_render_text_format_selector textFormatSelector,
  const level_edit_screen_state& screenState
);

void PlaySoundEffects
(
  const level_edit_screen_state& screenState, 
  global_sound_buffer_selector soundBuffers
);

bool ContinueRunning(const level_edit_screen_state& screenState);

#endif
