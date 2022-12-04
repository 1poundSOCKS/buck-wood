#include "pch.h"
#include "screen_render_data.h"
#include "screen_render.h"

auto screen_render_brush_white = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
auto screen_render_brush_grey = D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f);
auto screen_render_brush_green = D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f);
auto screen_render_brush_red = D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f);
auto screen_render_brush_yellow = D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f);
auto screen_render_brush_cyan = D2D1::ColorF(0.0f, 1.0f, 1.0f, 1.0f);

void CreateScreenRenderBrushes(ID2D1RenderTarget* renderTarget, std::back_insert_iterator<screen_render_brushes> brushInserter);
void CreateScreenRenderTextFormats(IDWriteFactory* dwriteFactory, std::back_insert_iterator<screen_render_text_formats> textFormatInserter);

void CreateScreenRenderBrushes(ID2D1RenderTarget* renderTarget, screen_render_brushes& brushes)
{
  brushes.clear();
  CreateScreenRenderBrushes(renderTarget, std::back_inserter(brushes));
}

void CreateScreenRenderTextFormats(IDWriteFactory* dwriteFactory, screen_render_text_formats& textFormats)
{
  textFormats.clear();
  CreateScreenRenderTextFormats(dwriteFactory, std::back_inserter(textFormats));
}

void CreateScreenRenderBrushes(ID2D1RenderTarget* renderTarget, std::back_insert_iterator<screen_render_brushes> brushInserter)
{
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_white);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_grey);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_green);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_red);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_yellow);
  brushInserter = CreateScreenRenderBrush(renderTarget, screen_render_brush_cyan);
}

void CreateScreenRenderTextFormats(IDWriteFactory* dwriteFactory, std::back_insert_iterator<screen_render_text_formats> textFormatInserter)
{
  textFormatInserter = CreateScreenRenderTextFormat(dwriteFactory, L"Verdana", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20);
}
