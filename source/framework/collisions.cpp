#include "pch.h"
#include "collisions.h"

[[nodiscard]] auto AddLineToInterceptCount(const game_line& line, const game_point& point) -> bool
{
  if( point.x >= line.start.x && point.x < line.end.x || point.x < line.start.x && point.x >= line.end.x )
  {
    float yIntercept = GetYIntercept(point.x, line);
    return yIntercept <= point.y;
  }
  return false;
}

[[nodiscard]] auto GetYIntercept(float x, const game_line& line) -> float
{
  float cx = line.end.x - line.start.x;
  float cy = line.end.y - line.start.y;
  float m = cy / cx;
  float b = line.start.y - m * line.start.x;
  return m * x + b;
}
