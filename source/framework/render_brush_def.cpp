#include "pch.h"
#include "render_brush_def.h"
#include "direct2d_functions.h"
#include "render_target.h"

render_brush_def::render_brush_def(D2D_COLOR_F color) : m_color { color.r, color.g, color.b, color.a }
{
}

render_brush_def::render_brush_def(D2D1::ColorF color) : m_color { color }
{
}

[[nodiscard]] auto render_brush_def::CreateBrush() const -> winrt::com_ptr<ID2D1SolidColorBrush>
{
  return direct2d::CreateScreenRenderBrush(render_target::get_raw(), m_color);
}

[[nodiscard]] auto render_brush_def::CreateBrush(ID2D1RenderTarget* renderTarget) const -> winrt::com_ptr<ID2D1SolidColorBrush>
{
  return direct2d::CreateScreenRenderBrush(renderTarget, m_color);
}
