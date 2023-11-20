#pragma once

#include "render_types.h"
#include "transformed_path_geometry.h"
#include "render_types.h"

class geometry_renderer
{

public:

  geometry_renderer(const render_brush& fillBrush);
  geometry_renderer(const render_brush& fillBrush, const render_brush& drawBrush, float drawWidth);

  auto Write(const path_geometry& geometry) const -> void;
  auto Write(const transformed_path_geometry& geometry) const -> void;

private:

  render_brush m_fillBrush;
  render_brush m_drawBrush;
  float m_drawWidth { 0 };

};
