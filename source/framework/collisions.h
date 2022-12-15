#ifndef _collisions_
#define _collisions_

#include "geometry.h"

bool AddLineToInterceptCount(const game_line& line, const game_point& point);
float GetYIntercept(float x, const game_line& line);

bool AnyPointInside(auto pointsBegin, auto pointsEnd, const std::vector<game_line>& area)
{
  for( auto point = pointsBegin; point != pointsEnd; ++point )
  {
    if( PointInside(*point, area.cbegin(), area.cend()) ) return true;
  }

  return false;
}

bool AllPointsInside(auto pointsBegin, auto pointsEnd, const std::vector<game_line>& area)
{
  for( auto point = pointsBegin; point != pointsEnd; ++point )
  {
    if( !PointInside(*point, area.cbegin(), area.cend()) ) return false;
  }

  return true;
}

bool PointInside(const game_point& point, auto linesBegin, auto linesEnd)
{
  auto matchingLines = GetLineInterceptCount(point, linesBegin, linesEnd);
  return ( matchingLines % 2 > 0 );
}

int GetLineInterceptCount(const game_point& point, auto linesBegin, auto linesEnd)
{  
  return std::accumulate(linesBegin, linesEnd, 0, [point](auto count, auto& line)
  {
    return AddLineToInterceptCount(line, point) ? count + 1 : count;
  });
}

[[nodiscard]] bool GetPointInsideShapeFlag(float x, float y, auto linesBegin, auto linesEnd)
{
  return ( GetLineInterceptCount({x, y}, linesBegin, linesEnd) % 2 == 1 );
}

#endif
