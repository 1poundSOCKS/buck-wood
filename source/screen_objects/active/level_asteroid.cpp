#include "pch.h"
#include "level_asteroid.h"
#include "perlin_simplex_noise.h"
#include "renderers.h"
#include "shape_generator.h"

level_asteroid::level_asteroid(game_rect rect)
{
  auto centrePoint = rect.CentrePoint();
  auto noise = psn::GetNoise(centrePoint.x, centrePoint.y);
  auto xRatio = noise > 0 ? noise / 2 + 0.5f : 1;
  auto yRatio = noise < 0 ? -noise / 2 + 0.5f : 1;
  rect.Resize(xRatio, yRatio);

  auto shapeGenerator = shape_generator { rect, 16 };

  auto view = shapeGenerator | std::ranges::views::transform([&centrePoint](auto point)
  {
    auto noise = 0.6f + ( psn::GetNoise(point.x, point.y) + 1 ) * 0.2f;

    auto cx = point.x - centrePoint.x;
    auto cy = point.y - centrePoint.y;

    return game_point { centrePoint.x + cx * noise, centrePoint.y + cy * noise };
  });
  
  m_geometry.Load(view);
}

[[nodiscard]] auto level_asteroid::Geometry() const -> const path_geometry&
{
  return m_geometry;
}
