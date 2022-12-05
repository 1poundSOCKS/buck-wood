#ifndef _main_menu_screen_
#define _main_menu_screen_

#include "screens/main_menu/main_menu_screen_state.h"
#include "sound_buffer_selector.h"
#include "screen_render_data.h"
#include "input_state.h"
#include "timers.h"

void UpdateScreenState
(
  main_menu_screen_state& screenState, 
  const screen_input_state& inputState, 
  const system_timer& timer
);

void RenderFrame
(
  ID2D1RenderTarget* renderTarget, 
  screen_render_brush_selector renderBrushSelector, 
  screen_render_text_format_selector textFormatSelector,
  const main_menu_screen_state& screenState
);

void PlaySoundEffects
(
  const main_menu_screen_state& screenState, 
  global_sound_buffer_selector soundBuffers
);

bool ContinueRunning(const main_menu_screen_state& screenState);

#endif
