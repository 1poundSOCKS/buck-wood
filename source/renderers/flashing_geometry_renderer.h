#pragma once

#include "framework.h"
#include "level_objects.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"
#include "cyclic_interval.h"

class flashing_geometry_renderer
{

public:

  flashing_geometry_renderer(color_scale colorScale) : m_targetBrushes { colorScale }
  {
  }

  auto Write(auto&& object, ID2D1Geometry* geometry) const -> void;

private:

  color_scale_brushes m_targetBrushes;

};

inline auto flashing_geometry_renderer::Write(auto&& object, ID2D1Geometry* geometry) const -> void
{
  constexpr static float cycleLength = 5.0f;

  cyclic_interval m_cyclicIntervalReversal { object.Health() * cycleLength };
  cyclic_interval m_cyclicInterval { object.Health() * cycleLength / 2.0f };

  auto reversedCycle = static_cast<int>(m_cyclicIntervalReversal.get(object.Age()) * 2.0f) ? true : false;
  auto cyclicAge = reversedCycle ? m_cyclicInterval.get(object.Age()) : 1.0f - m_cyclicInterval.get(object.Age());
  geometry_renderer::Write(geometry, m_targetBrushes[cyclicAge].get(), m_targetBrushes[0.0f].get(), 10);
}
