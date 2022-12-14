#include "pch.h"
#include "game_math.h"

int CalculateAngle(float x1, float y1, float x2, float y2)
{
  float radians = atan2(y2 - y1, x2 - x1);
  float degrees = RADTODEG(radians);
  return degrees + 90.5;
}

bool AllPointsInside(const std::vector<game_point>::const_iterator& begin, const std::vector<game_point>::const_iterator& end, const std::vector<game_line>& area)
{
  for( auto point = begin; point != end; ++point )
  {
    if( !PointInside(*point, area) ) return false;
  }

  return true;
}

bool AnyPointInside(const std::vector<game_point>::const_iterator& begin, const std::vector<game_point>::const_iterator& end, const std::vector<game_line>& area)
{
  for( auto point = begin; point != end; ++point )
  {
    if( PointInside(*point, area) ) return true;
  }

  return false;
}

bool PointInside(const game_point& point, const std::vector<game_line>& lines)
{
  auto matchingLines = GetLineInterceptCount(point, lines);  
  return ( matchingLines % 2 > 0 );
}

int GetLineInterceptCount(const game_point& point, const std::vector<game_line>& lines)
{
  return GetLineInterceptCount(point, lines.cbegin(), lines.cend());
}

int GetLineInterceptCount(const game_point& point, std::vector<game_line>::const_iterator linesBegin, std::vector<game_line>::const_iterator linesEnd)
{  
  return std::accumulate(linesBegin, linesEnd, 0, [point](auto count, auto& line)
  {
    return AddLineToInterceptCount(line, point) ? count + 1 : count;
  });
}

bool AddLineToInterceptCount(const game_line& line, const game_point& point)
{
  if( point.x >= line.start.x && point.x < line.end.x || point.x < line.start.x && point.x >= line.end.x )
  {
    float yIntercept = GetYIntercept(point.x, line);
    return yIntercept <= point.y;
  }
  return false;
}

float GetYIntercept(float x, const game_line& line)
{
  float cx = line.end.x - line.start.x;
  float cy = line.end.y - line.start.y;
  float m = cy / cx;
  float b = line.start.y - m * line.start.x;
  return m * x + b;
}

float GetDistanceBetweenPoints(float x1, float y1, float x2, float y2)
{
  float cx = x2 - x1;
  float cy = y2 - y1;
  return sqrt( cx * cx + cy * cy );
}
