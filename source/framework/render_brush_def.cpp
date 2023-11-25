#include "pch.h"
#include "render_brush_def.h"
#include "directx_functions.h"
#include "render_target.h"

render_brush_def::render_brush_def(D2D_COLOR_F color) : m_color { color.r, color.g, color.b, color.a }
{
}

render_brush_def::render_brush_def(D2D1::ColorF color) : m_color { color }
{
}

[[nodiscard]] auto render_brush_def::CreateBrush() const -> winrt::com_ptr<ID2D1SolidColorBrush>
{
  return CreateScreenRenderBrush(render_target::renderTarget().get(), m_color);
}

[[nodiscard]] auto render_brush_def::CreateBrush(ID2D1RenderTarget* renderTarget) const -> winrt::com_ptr<ID2D1SolidColorBrush>
{
  return CreateScreenRenderBrush(renderTarget, m_color);
}
