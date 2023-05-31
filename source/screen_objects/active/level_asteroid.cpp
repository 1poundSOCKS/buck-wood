#include "pch.h"
#include "level_asteroid.h"
#include "render_brush_defs.h"
#include "perlin_simplex_noise.h"
#include "renderers.h"

level_asteroid::level_asteroid(game_rect rect)
{
  auto width = ( rect.bottomRight.x - rect.topLeft.x ) / 2.0f;
  auto height = ( rect.bottomRight.y - rect.topLeft.y ) / 2.0f;

  auto centrePoint = rect.CentrePoint();
  auto x = centrePoint.x;
  auto y = centrePoint.y;

  m_object.Reserve(360 / 30);

  for( int angle = 0; angle < 360; angle += 30 )
  {
    auto angleInRadians = DEGTORAD(angle);

    auto cx = width * sin(angleInRadians);
    auto cy = height * cos(angleInRadians);

    auto noise = psn::GetNoise(static_cast<float>(x + cx), static_cast<float>(y + cy));
    noise = ( noise + 5.0f ) / 6.0f;

    m_object += {x + cx * noise, y + cy * noise};
  }

  m_object.Finalize();
  m_geometry.Load(m_object.points);
}

[[nodiscard]] auto level_asteroid::Geometry() const -> const path_geometry&
{
  return m_geometry;
}
