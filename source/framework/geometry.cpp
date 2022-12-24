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

[[nodiscard]] game_rect GetBoundingRect(game_line line) [[nothrow]]
{
  return {
    min(line.start.x, line.end.x),
    min(line.start.y, line.end.y),
    max(line.start.x, line.end.x),
    max(line.start.y, line.end.y)
  };
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
