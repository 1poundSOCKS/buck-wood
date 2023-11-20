#pragma once

#include "render_types.h"
#include "path_geometry.h"
#include "render_types.h"

class geometry_renderer
{

public:

  geometry_renderer(const render_brush& fillBrush);
  geometry_renderer(const render_brush& fillBrush, const render_brush& drawBrush, float drawWidth);

  auto Write(const path_geometry& pathGeometry) const -> void;

private:

  render_brush m_fillBrush;
  render_brush m_drawBrush;
  float m_drawWidth { 0 };

};
