#include "pch.h"
#include "geometry.h"

int CalculateAngle(float x1, float y1, float x2, float y2)
{
  float radians = atan2(y2 - y1, x2 - x1);
  float degrees = RADTODEG(radians);
  return degrees + 90.5;
}

float GetDistanceBetweenPoints(float x1, float y1, float x2, float y2)
{
  float cx = x2 - x1;
  float cy = y2 - y1;
  return sqrt( cx * cx + cy * cy );
}

[[nodiscard]] auto GetBoundingRect(game_line line) -> game_rect [[nothrow]]
{
  return {
    min(line.start.x, line.end.x),
    min(line.start.y, line.end.y),
    max(line.start.x, line.end.x),
    max(line.start.y, line.end.y)
  };
}

[[nodiscard]] auto GetBoundingRect(game_rect rect1, game_rect rect2) -> game_rect [[nothrow]]
{
  return {
    min(rect1.topLeft.x, rect2.topLeft.x),
    min(rect1.topLeft.y, rect2.topLeft.y),
    max(rect1.bottomRight.x, rect2.bottomRight.x),
    max(rect1.bottomRight.y, rect2.bottomRight.y)
  };
}

[[nodiscard]] auto GetBoundingRect(const game_closed_object& object) -> game_rect [[nothrow]]
{
  auto firstPoint = object.points.cbegin();
  return std::reduce(
    std::next(firstPoint), object.points.cend(), 
    game_rect {firstPoint->x , firstPoint->y, firstPoint->x, firstPoint->y},
    [](game_rect rect, game_point point) -> game_rect
    {
      return {
        min(point.x, rect.topLeft.x),
        min(point.y, rect.topLeft.y),
        max(point.x, rect.bottomRight.x),
        max(point.y, rect.bottomRight.y)
      };
    }
  );
}

[[nodiscard]] bool DoOverlap(game_rect rect1, game_rect rect2) [[nothrow]]
{
  return !(
    ( rect1.topLeft.x < rect2.topLeft.x && rect1.bottomRight.x < rect2.topLeft.x ) ||
    ( rect1.topLeft.x > rect2.topLeft.x && rect1.bottomRight.x > rect2.topLeft.x ) ||
    ( rect1.topLeft.y < rect2.topLeft.y && rect1.bottomRight.y < rect2.topLeft.y ) ||
    ( rect1.topLeft.y > rect2.topLeft.y && rect1.bottomRight.y > rect2.topLeft.y )
  );
}

[[nodiscard]] auto GetCentrePoint(game_rect rect) -> game_point [[nothrow]]
{
  return {
    (rect.topLeft.x + rect.bottomRight.x) / 2,
    (rect.topLeft.y + rect.bottomRight.y) / 2
  };
}
