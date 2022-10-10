#include "game_math.h"

int CalculateAngle(float x1, float y1, float x2, float y2)
{
  float radians = atan2(y2 - y1, x2 - x1);
  float degrees = RADTODEG(radians);
  return degrees + 90.5;
}

bool PointsInside(const std::list<game_point>& points, const game_shape& shape)
{
  int matchingPointCount = 0;
  for( const auto& point: points )
  {
    if( PointInside(point, shape) ) matchingPointCount++;
  }

  return matchingPointCount == points.size();
}

bool PointInside(const std::list<game_point>& points, const game_shape& shape)
{
  int matchingPointCount = 0;
  for( const auto& point: points )
  {
    if( PointInside(point, shape) ) return true;
  }

  return false;
}

bool PointInside(const game_point& point, const game_shape& shape)
{
  int matchingLines = 0;
  for( const auto& line: shape.lines )
  {
    if( AddLineToInterceptCount(line, point) ) matchingLines++;
  }
  
  return ( matchingLines % 2 > 0 );
}

bool AddLineToInterceptCount(const game_line& line, const game_point& point)
{
  if( point.x >= line.first.x && point.x < line.second.x || point.x < line.first.x && point.x >= line.second.x )
  {
    float yIntercept = GetYIntercept(point.x, line);
    return yIntercept <= point.y;
  }
  return false;
}

float GetYIntercept(float x, const game_line& line)
{
  float cx = line.second.x - line.first.x;
  float cy = line.second.y - line.first.y;
  float m = cy / cx;
  float b = line.first.y - m * line.first.x;
  return m * x + b;
}
