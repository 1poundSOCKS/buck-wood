#include "pch.h"
#include "render_brush_def.h"
#include "screen_render.h"

render_brush_def::render_brush_def(D2D1::ColorF color) : m_color(color)
{
}

auto render_brush_def::CreateBrush(ID2D1RenderTarget* renderTarget) const -> winrt::com_ptr<ID2D1SolidColorBrush>
{
  return CreateScreenRenderBrush(renderTarget, m_color);
}
