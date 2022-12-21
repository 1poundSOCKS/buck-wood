#include "pch.h"
#include "screen_render_data.h"
#include "screen_render.h"

auto screen_render_brush_white      = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
auto screen_render_brush_grey       = D2D1::ColorF(0.4f, 0.4f, 0.4f, 1.0f);
auto screen_render_brush_dark_grey  = D2D1::ColorF(0.1f, 0.1f, 0.1f, 1.0f);
auto screen_render_brush_green      = D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f);
auto screen_render_brush_red        = D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f);
auto screen_render_brush_yellow     = D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f);
auto screen_render_brush_cyan       = D2D1::ColorF(0.0f, 1.0f, 1.0f, 1.0f);
auto screen_render_brush_brown      = D2D1::ColorF(0.6f, 0.3f, 0.0f, 1.0f);

[[nodiscard]] auto CreateScreenRenderBrushes(ID2D1RenderTarget* renderTarget) -> screen_render_brushes
{
  return {
    CreateScreenRenderBrush(renderTarget, screen_render_brush_white),
    CreateScreenRenderBrush(renderTarget, screen_render_brush_grey),
    CreateScreenRenderBrush(renderTarget, screen_render_brush_dark_grey),
    CreateScreenRenderBrush(renderTarget, screen_render_brush_green),
    CreateScreenRenderBrush(renderTarget, screen_render_brush_red),
    CreateScreenRenderBrush(renderTarget, screen_render_brush_yellow),
    CreateScreenRenderBrush(renderTarget, screen_render_brush_cyan),
    CreateScreenRenderBrush(renderTarget, screen_render_brush_brown)
  };
}

[[nodiscard]] auto CreateScreenRenderTextFormats(IDWriteFactory* dwriteFactory) -> screen_render_text_formats
{
  return {
    CreateScreenRenderTextFormat(dwriteFactory, L"Verdana", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20),
    CreateScreenRenderTextFormat(dwriteFactory, L"Verdana", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20),
    CreateScreenRenderTextFormat(dwriteFactory, L"Franklin Gothic", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 80),
    CreateScreenRenderTextFormat(dwriteFactory, L"Franklin Gothic", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 100)
  };
}
