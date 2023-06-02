#include "pch.h"
#include "level_asteroid.h"
#include "render_brush_defs.h"
#include "perlin_simplex_noise.h"
#include "renderers.h"
#include "shape_generator.h"

level_asteroid::level_asteroid(game_rect rect)
{
  m_geometry.Load( shape_generator<noisy_shape> { rect, 12 } );
}

[[nodiscard]] auto level_asteroid::Geometry() const -> const path_geometry&
{
  return m_geometry;
}
