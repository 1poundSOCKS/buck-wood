#include "pch.h"
#include "level_asteroid.h"
#include "perlin_simplex_noise.h"
#include "renderers.h"
#include "shape_generator.h"

level_asteroid::level_asteroid(float x, float y, float maxWidth, float maxHeight)
{
  auto xRatioNoise = psn::GetNoise(x / 7, y / 7);
  auto yRatioNoise = psn::GetNoise(x / 13, y / 13);
  auto angleNoise = psn::GetNoise(x / 17, y / 17);

  auto xRatio = ( xRatioNoise + 1 ) / 4 + 0.5f;
  auto yRatio = ( yRatioNoise + 1 ) / 4 + 0.5f;
  auto angle = ( angleNoise + 1 ) / 2 * 360;
  
  auto width = maxWidth * xRatio;
  auto height = maxHeight * yRatio;

  auto shapeGenerator = shape_generator { 0, 0, width, height, 10 };

  auto irregularShape = shapeGenerator | std::ranges::views::transform([x, y](auto point)
  {
    auto noise = 0.7f + ( psn::GetNoise(x + point.x, y + point.y) + 1 ) * 0.1f;
    return game_point { point.x * noise, point.y * noise };
  });
  
  m_geometry.Load(irregularShape);
  
  auto transform = D2D1::Matrix3x2F::Rotation(angle) * D2D1::Matrix3x2F::Translation(x, y);
  m_transformedGeometry = transformed_path_geometry { m_geometry, transform };
}

[[nodiscard]] auto level_asteroid::Geometry() const -> const transformed_path_geometry&
{
  return m_transformedGeometry;
}
