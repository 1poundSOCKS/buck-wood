#include "pch.h"
#include "game_level_object_generator.h"
#include "framework.h"
#include "perlin_simplex_noise.h"

game_level_object_generator::game_level_object_generator(game_rect rect) : m_rect(rect)
{
}

auto game_level_object_generator::InsertInto(std::back_insert_iterator<asteroid_collection> asteroidInserter, std::back_insert_iterator<target_collection> targetInserter) const -> void
{
  rect_generator::collection largeAsteroidRects;
  rect_generator::collection smallAsteroidRects;
  rect_generator::collection targetRects;

  rect_generator::collection emptyLargeRects;

  rect_generator largeRectGenerator(m_rect, 400.0f);
  largeRectGenerator.Get(std::back_inserter(largeAsteroidRects), std::back_inserter(emptyLargeRects), [](float noise) -> bool { return noise < -0.80f; });

  for( auto largeRect : emptyLargeRects )
  {
    rect_generator smallRectGenerator(largeRect, 100.0f);
    smallRectGenerator.Get(std::back_inserter(smallAsteroidRects), [](float noise) -> bool { return noise < -0.86f; });
    smallRectGenerator.Get(std::back_inserter(targetRects), [](float noise) -> bool { return noise > 0.967f; });
  }

  std::transform(largeAsteroidRects.cbegin(), largeAsteroidRects.cend(), asteroidInserter, [this](auto rect)
  {
    return CreateAsteroid(rect);
  });

  std::transform(smallAsteroidRects.cbegin(), smallAsteroidRects.cend(), asteroidInserter, [this](auto rect)
  {
    return CreateAsteroid(rect);
  });

  std::transform(targetRects.cbegin(), targetRects.cend(), targetInserter, [](auto rect)
  {
    auto centrePoint = rect.CentrePoint();
    return level_target { centrePoint.x, centrePoint.y };
  });
}

auto game_level_object_generator::CreateAsteroid(game_rect rect) const -> game_closed_object
{
  auto radiusX = ( rect.bottomRight.x - rect.topLeft.x ) / 2.0f;
  auto radiusY = ( rect.bottomRight.y - rect.topLeft.y ) / 2.0f;

  auto centrePoint = rect.CentrePoint();
  auto x = centrePoint.x;
  auto y = centrePoint.y;

  std::vector<game_point> points;

  for( int angle = 0; angle < 360; angle += 40 )
  {
    auto angleInRadians = DEGTORAD(angle);

    auto cx = radiusX * sin(angleInRadians);
    auto cy = radiusY * cos(angleInRadians);

    auto noise = psn::GetNoise(static_cast<float>(x + cx), static_cast<float>(y + cy));
    noise = ( noise + 5.0f ) / 6.0f;

    points.emplace_back( game_point {x + cx * noise, y + cy * noise} );
  }

  game_closed_object asteroid;
  asteroid.Load(points.cbegin(), points.cend());

  return asteroid;
}
