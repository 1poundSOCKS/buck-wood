#ifndef _collisions_
#define _collisions_

#include "geometry.h"

[[nodiscard]] bool AddLineToInterceptCount(const game_line& line, const game_point& point) [[nothrow]];
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

[[nodiscard]] auto PointCollisionCount(auto pointsBegin, auto pointsEnd, auto linesBegin, auto linesEnd) -> int
{
  return std::accumulate(pointsBegin, pointsEnd, 0, [linesBegin, linesEnd](auto total, auto point)
  {
    return total += PointInside(point, linesBegin, linesEnd) ? 1 : 0;
  });
}

bool PointInside(const game_point& point, auto linesBegin, auto linesEnd)
{
  auto matchingLines = GetLineInterceptCount(point, linesBegin, linesEnd);
  return ( matchingLines % 2 > 0 );
}

  
[[nodicard]] auto PointInsideCount(auto beginIterator, auto endIterator, const auto& linesCollection) -> int
{
  return std::reduce(
    beginIterator, endIterator, 0,
    [&linesCollection](auto total, auto point) -> int
    {
      return PointInside(point, linesCollection.cbegin(), linesCollection.cend()) ? total + 1 : total;
    }
  );
}

[[nodicard]] auto PointInsideCount(auto beginIterator, auto endIterator, game_rect rect) -> int
{
  std::vector<game_line> lines;
  CreateConnectedLines(rect, std::back_inserter(lines));
  return PointInsideCount(beginIterator, endIterator, lines);
}

int GetLineInterceptCount(const game_point& point, auto linesBegin, auto linesEnd)
{  
  return std::accumulate(linesBegin, linesEnd, 0, [point](auto count, auto& line)
  {
    return AddLineToInterceptCount(line, point) ? count + 1 : count;
  });
}

[[nodiscard]] auto PointInsideObject(float x, float y, auto linesBegin, auto linesEnd) -> bool
{
  return GetLineInterceptCount({x, y}, linesBegin, linesEnd) % 2 == 1;
}

[[nodiscard]] auto ObjectsHaveCollided(const auto& object1, const auto& object2) -> bool
{
  return std::reduce(
    object1.points.cbegin(), 
    object1.points.cend(), 
    false, 
    [&object2](auto hit, auto point)
    {
      return hit || PointInsideObject(point.x, point.y, object2.lines.cbegin(), object2.lines.cend());
    }
  ) ||
  std::reduce(
    object2.points.cbegin(), 
    object2.points.cend(), 
    false, 
    [&object1](auto hit, auto point)
    {
      return hit || PointInsideObject(point.x, point.y, object1.lines.cbegin(), object1.lines.cend());
    }
  );
}

#endif
