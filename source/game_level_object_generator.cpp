#include "pch.h"
#include "game_level_object_generator.h"
#include "framework.h"
#include "perlin_simplex_noise.h"

constexpr auto GetLargeGridColumnCount() -> int
{
  return 10;
}

constexpr auto GetLargeGridRowCount() -> int
{
  return 10;
}

constexpr auto GetSmallGridColumnCount() -> int
{
  return 4;
}

constexpr auto GetSmallGridRowCount() -> int
{
  return 4;
}

game_level_object_generator::game_level_object_generator(game_rect rect, std::function<bool(float)> generateObject) : 
  m_generateObject(generateObject), m_rectGenerator(rect, GetLargeGridColumnCount(), GetLargeGridRowCount())
{
}

auto game_level_object_generator::InsertInto(std::back_insert_iterator<asteroid_collection> inserter) const -> void
{
  rect_generator::collection rects;
  m_rectGenerator.Get(std::back_inserter(rects), m_generateObject);
  std::transform(rects.cbegin(), rects.cend(), inserter, [this](auto rect)
  {
    return CreateAsteroid(rect);
  });
}

auto game_level_object_generator::InsertInto(std::back_insert_iterator<target_collection> inserter) const -> void
{
  rect_generator::collection rects;
  m_rectGenerator.Get(std::back_inserter(rects), m_generateObject);
  std::transform(rects.cbegin(), rects.cend(), inserter, [](auto rect)
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
