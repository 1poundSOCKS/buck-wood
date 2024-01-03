#include "pch.h"
#include "geometry_renderer.h"
#include "framework.h"

geometry_renderer::geometry_renderer(const render_brush& fillBrush) : m_fillBrush { fillBrush }
{
}

geometry_renderer::geometry_renderer(const render_brush& fillBrush, const render_brush& drawBrush, float drawWidth) : m_fillBrush { fillBrush }, m_drawBrush { drawBrush }, m_drawWidth { drawWidth }
{
}

geometry_renderer::geometry_renderer(const render_brush& drawBrush, float drawWidth) : m_drawBrush { drawBrush }, m_drawWidth { drawWidth }
{
}

auto geometry_renderer::Write(ID2D1Geometry* geometry) const -> void
{
  if( m_fillBrush )
  {
    render_target::get()->FillGeometry(geometry, m_fillBrush.get());
  }

  if( m_drawBrush )
  {
    render_target::get()->DrawGeometry(geometry, m_drawBrush.get(), m_drawWidth);
  }
}
