#ifndef _geometry_
#define _geometry_

#define PI 3.14159265f
#define DEGTORAD(D)((D * PI) / 180.0f)
#define RADTODEG(R)((180.0f * R) / PI)

[[nodiscard]] inline auto CalculateVectorX(float value, float angle) -> float
{
  return value * sin(DEGTORAD(angle));
}

[[nodiscard]] inline auto CalculateVectorY(float value, float angle) -> float
{
  return value * cos(DEGTORAD(angle));
}

struct game_point
{
  float x = 0;
  float y = 0;
};

struct game_line
{
  game_point start;
  game_point end;
};

struct game_rect
{
  game_rect();
  game_rect(float left, float top, float right, float bottom);
  game_rect(game_point topLeft, game_point bottomRight);
  game_rect(D2D1_RECT_F rect);

  [[nodiscard]] auto Width() const -> float;
  [[nodiscard]] auto Height() const -> float;
  [[nodiscard]] auto CentrePoint() const -> game_point;

  game_point topLeft = { 0, 0 };
  game_point bottomRight = { 0, 0 };
};

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

  auto GetBoundingRect() const -> game_rect;

  std::vector<game_point> points;
  std::vector<game_line> lines;
};

struct game_open_object
{
  std::vector<game_point> points;
  std::vector<game_line> lines;
};

auto CalculateAngle(float x1, float y1, float x2, float y2) -> float;

float GetDistanceBetweenPoints(float x1, float y1, float x2, float y2);

[[nodiscard]] auto GetBoundingRect(game_line line) -> game_rect ;
[[nodiscard]] auto GetBoundingRect(game_rect rect1, game_rect rect2) -> game_rect;
// [[nodiscard]] auto GetBoundingRect(const game_closed_object& object) -> game_rect;
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

#endif
