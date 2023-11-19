#pragma once

#include "render_types.h"
#include "path_geometry.h"

class geometry_renderer
{

public:

  auto Write(const path_geometry& pathGeometry, const render_brush& fillBrush, const render_brush& drawBrush, DWORD drawWidth) const -> void;
  auto Write(const path_geometry& pathGeometry, const render_brush& fillBrush) const -> void;

};

inline auto geometry_renderer::Write(const path_geometry& pathGeometry, const render_brush& fillBrush, const render_brush& drawBrush, DWORD drawWidth) const -> void
{
}
