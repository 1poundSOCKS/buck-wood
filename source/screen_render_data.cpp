#include "pch.h"
#include "screen_render_data.h"
#include "screen_render.h"
#include "render_defs.h"

auto CreateScreenRenderBrushes(ID2D1RenderTarget* renderTarget, std::back_insert_iterator<screen_render_brushes> brushInserter) -> void
{
  brushInserter = screen_render_brush_white.CreateBrush(renderTarget);
  brushInserter = screen_render_brush_grey.CreateBrush(renderTarget);
  brushInserter = screen_render_brush_dark_grey.CreateBrush(renderTarget);
  brushInserter = screen_render_brush_green.CreateBrush(renderTarget);
  brushInserter = screen_render_brush_red.CreateBrush(renderTarget);
  brushInserter = screen_render_brush_yellow.CreateBrush(renderTarget);
  brushInserter = screen_render_brush_cyan.CreateBrush(renderTarget);
  brushInserter = screen_render_brush_brown.CreateBrush(renderTarget);
}

// auto CreateScreenRenderTextFormats(IDWriteFactory* dwriteFactory, std::back_insert_iterator<screen_render_text_formats> textFormatInserter) -> void
// {
//   textFormatInserter = render_text_format_prompt.CreateTextFormat(dwriteFactory);
//   textFormatInserter = render_text_format_main_menu.CreateTextFormat(dwriteFactory);
//   textFormatInserter = render_text_format_play_screen_state.CreateTextFormat(dwriteFactory);
//   textFormatInserter = render_text_format_play_screen_timer.CreateTextFormat(dwriteFactory);
// }
