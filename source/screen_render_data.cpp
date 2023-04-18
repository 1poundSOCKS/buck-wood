#include "pch.h"
#include "screen_render_data.h"
#include "screen_render.h"
#include "render_defs.h"

auto CreateScreenRenderBrushes(ID2D1RenderTarget* renderTarget, std::back_insert_iterator<screen_render_brushes> brushInserter) -> void
{
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_white);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_grey);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_dark_grey);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_green);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_red);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_yellow);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_cyan);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_brown);
}

auto CreateScreenRenderTextFormats(IDWriteFactory* dwriteFactory, std::back_insert_iterator<screen_render_text_formats> textFormatInserter) -> void
{
  textFormatInserter = CreateScreenRenderTextFormat(dwriteFactory, L"Verdana", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20);
  textFormatInserter = CreateScreenRenderTextFormat(dwriteFactory, L"Verdana", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20);
  textFormatInserter = CreateScreenRenderTextFormat(dwriteFactory, L"Franklin Gothic", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 80);
  textFormatInserter = CreateScreenRenderTextFormat(dwriteFactory, L"Franklin Gothic", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 100);
}
