#ifndef _collisions_
#define _collisions_

#include "geometry.h"

[[nodiscard]] auto AddLineToInterceptCount(const game_line& line, const game_point& point) -> bool;
[[nodiscard]] auto GetYIntercept(float x, const game_line& line) -> float;

void CreateConnectedLines(auto begin, auto end, auto lines, bool loop=true)
{
  if( std::distance(begin, end) > 0 )
  {
    std::transform(std::next(begin), end, begin, lines, [](const auto& point2, const auto& point1)
    {
      return game_line { point1.x, point1.y, point2.x, point2.y };
    });

    if( loop )
    {
      auto last = std::prev(end);
      lines = game_line { last->x, last->y, begin->x, begin->y };
    }
  }
};

void CreateConnectedLines(game_rect rect, auto lines)
{
  auto points = std::array {
    rect.topLeft,
    game_point { rect.bottomRight.x, rect.topLeft.y },
    rect.bottomRight,
    game_point { rect.topLeft.x, rect.bottomRight.y }
  };

  CreateConnectedLines(points.cbegin(), points.cend(), lines);
};

struct game_closed_object
{
  game_closed_object()
  {
  }

  template <typename point_iterator_type>
  game_closed_object(point_iterator_type begin, point_iterator_type end)
  {
    Load(begin, end);
  }

  template <typename point_iterator_type>
  auto Load(point_iterator_type begin, point_iterator_type end) -> void
  {
    Clear();
    std::copy(begin, end, std::back_inserter(points));
    CreateConnectedLines(points.cbegin(), points.cend(), std::back_inserter(lines));
  }

  auto Clear() -> void
  {
    points.clear();
    lines.clear();
  }

  auto Reserve(size_t size) -> void
  {
    points.reserve(size);
    lines.reserve(size);
  }

  auto GetBoundingRect() const -> game_rect;

  auto operator+=(game_point&& point) -> void
  {
    points.emplace_back(point);
  }

  auto operator+=(game_point point) -> void
  {
    points.push_back(point);
  }

  auto Finalize() -> void
  {
    CreateConnectedLines(points.cbegin(), points.cend(), std::back_inserter(lines));
  }

  std::vector<game_point> points;
  std::vector<game_line> lines;
};

struct game_open_object
{
  std::vector<game_point> points;
  std::vector<game_line> lines;
};

[[nodiscard]] auto GetBoundingRect(game_line line) -> game_rect ;
[[nodiscard]] auto GetBoundingRect(game_rect rect1, game_rect rect2) -> game_rect;
[[nodiscard]] auto DoOverlap(game_rect rect1, game_rect rect2) -> bool;
[[nodiscard]] auto GetCentrePoint(game_rect rect) -> game_point;

void TransformPoints(auto begin, auto end, auto transformedPoints, const D2D1::Matrix3x2F& transform)
{
  std::transform(begin, end, transformedPoints, [transform](const auto& point){
    auto outPoint = transform.TransformPoint({point.x, point.y});
    return decltype(transformedPoints)::container_type::value_type ( outPoint.x, outPoint.y );
  });
}

[[nodiscard]] auto LoadOpenObject(auto beginPoint, auto endPoint) -> game_open_object
{
  game_open_object openObject;
  std::copy(beginPoint, endPoint, std::back_inserter(openObject.points));
  CreateConnectedLines(openObject.points.cbegin(), openObject.points.cend(), std::back_inserter(openObject.lines), false);
  return openObject;
}

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

inline [[nodiscard]] auto PointInside(float x, float y, const game_closed_object& object) -> bool
{
  return PointInside({ x, y }, object.lines.cbegin(), object.lines.cend());
}

inline auto HitByBullet(game_closed_object&) -> void
{
}

[[nodiscard]] auto PointInsideCount(auto beginIterator, auto endIterator, const auto& linesCollection) -> int
{
  return std::reduce(
    beginIterator, endIterator, 0,
    [&linesCollection](auto total, auto point) -> int
    {
      return PointInside(point, linesCollection.cbegin(), linesCollection.cend()) ? total + 1 : total;
    }
  );
}

[[nodiscard]] auto PointInsideCount(auto beginIterator, auto endIterator, game_rect rect) -> int
{
  std::vector<game_line> lines;
  CreateConnectedLines(rect, std::back_inserter(lines));
  return PointInsideCount(beginIterator, endIterator, lines);
}

[[nodiscard]] auto GetLineInterceptCount(const game_point& point, auto linesBegin, auto linesEnd) -> int
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
