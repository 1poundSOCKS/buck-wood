#include "pch.h"
#include "level_target.h"
#include "render_brush_defs.h"
#include "renderers.h"
#include "shape_generator.h"

level_target::level_target(float x, float y) : m_position { x, y }
{
  game_rect rect = { { x - 20, y - 20 }, { x + 20, y + 20} };
  m_geometry.Load( shape_generator<clean_shape> { rect, 6 } );
}

[[nodiscard]] auto level_target::Position() const -> game_point
{
  return m_position;
}

[[nodiscard]] auto level_target::IsActivated() const -> bool
{
  return m_activated;
}

[[nodiscard]] auto level_target::Geometry() const -> const path_geometry&
{
  return m_geometry;
}

auto level_target::Activate() -> void
{
  m_activated = true;
}
