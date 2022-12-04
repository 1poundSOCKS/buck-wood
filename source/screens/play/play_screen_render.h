#ifndef _play_screen_render_
#define _play_screen_render_

#include "render.h"
#include "play_screen_state.h"
#include "screen_render_data.h"

void RenderFrame(
  ID2D1RenderTarget* renderTarget, 
  screen_render_brush_selector renderBrushSelector, 
  screen_render_text_format_selector textFormatSelector,
  const play_screen_state& screenState);

#endif
