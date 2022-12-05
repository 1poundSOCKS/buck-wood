#ifndef _play_screen_
#define _play_screen_

#include "play/play_screen_state.h"
#include "sound_buffer_selector.h"
#include "screen_render_data.h"

void UpdateScreenState(
  play_screen_state& screenState, 
  const screen_input_state& inputState, 
  const system_timer& timer);

void RenderFrame(
  ID2D1RenderTarget* renderTarget, 
  screen_render_brush_selector renderBrushSelector, 
  screen_render_text_format_selector textFormatSelector,
  const play_screen_state& screenState);

void PlaySoundEffects(
  const play_screen_state& screenState, 
  global_sound_buffer_selector soundBuffers);

bool ContinueRunning(const play_screen_state& screenState);

#endif
