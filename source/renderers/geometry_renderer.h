#pragma once

#include "render_types.h"

class geometry_renderer
{

public:

  geometry_renderer(const render_brush& fillBrush);
  geometry_renderer(const render_brush& fillBrush, const render_brush& drawBrush, float drawWidth);
  geometry_renderer(const render_brush& drawBrush, float drawWidth);

  auto Write(ID2D1Geometry* geometry) const -> void;

  static auto Write(ID2D1Geometry* geometry, ID2D1SolidColorBrush* fillBrush, ID2D1SolidColorBrush* drawBrush, float drawWidth) -> void;

private:

  render_brush m_fillBrush;
  render_brush m_drawBrush;
  float m_drawWidth { 0 };

};
