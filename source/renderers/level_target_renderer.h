#pragma once

#include "framework.h"
#include "level_objects.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"
#include "cyclic_interval.h"

class level_target_renderer
{

public:

  auto Write(const level_target& object, ID2D1Geometry* geometry) const -> void;

private:

  color_scale_brushes m_targetBrushes { color_scale { screen_render_brush_red.Get(), screen_render_brush_black.Get(), 10 } };

};

inline auto level_target_renderer::Write(const level_target& object, ID2D1Geometry* geometry) const -> void
{
  constexpr static float cycleLength = 5.0f;

  cyclic_interval m_cyclicIntervalReversal { object.Health() * cycleLength };
  cyclic_interval m_cyclicInterval { object.Health() * cycleLength / 2.0f };

  auto reversedCycle = static_cast<int>(m_cyclicIntervalReversal.get(object.Age()) * 2.0f) ? true : false;
  auto cyclicAge = reversedCycle ? m_cyclicInterval.get(object.Age()) : 1.0f - m_cyclicInterval.get(object.Age());
  geometry_renderer::Write(geometry, m_targetBrushes[cyclicAge].get(), m_targetBrushes[0.0f].get(), 10);
}
