#include "pch.h"
#include "geometry_renderer.h"

geometry_renderer::geometry_renderer(const render_brush& fillBrush) : m_fillBrush { fillBrush }
{
}

geometry_renderer::geometry_renderer(const render_brush& fillBrush, const render_brush& drawBrush, float drawWidth) : m_fillBrush { fillBrush }, m_drawBrush { drawBrush }, m_drawWidth { drawWidth }
{
}

auto geometry_renderer::Write(const path_geometry& pathGeometry) const -> void
{
  render_target::renderTarget()->FillGeometry(pathGeometry.Get(), m_fillBrush.get());

  if( m_drawBrush )
  {
    render_target::renderTarget()->DrawGeometry(pathGeometry.Get(), m_drawBrush.get(), m_drawWidth);
  }
}
