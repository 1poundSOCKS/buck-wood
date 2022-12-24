#ifndef _geometry_
#define _geometry_

#define PI 3.14159265
#define DEGTORAD(D)((D * PI) / 180.0)
#define RADTODEG(R)((180.0 * R) / PI)

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
  game_point topLeft;
  game_point bottomRight;
};

int CalculateAngle(float x1, float y1, float x2, float y2);

float GetDistanceBetweenPoints(float x1, float y1, float x2, float y2);

[[nodiscard]] auto GetBoundingRect(game_line line) -> game_rect [[nothrow]];
[[nodiscard]] auto DoOverlap(game_rect rect1, game_rect rect2) -> bool [[nothrow]];
[[nodiscard]] auto GetCentrePoint(game_rect rect) -> game_point [[nothrow]];

void TransformPoints(auto begin, auto end, auto transformedPoints, const D2D1::Matrix3x2F& transform)
{
  std::transform(begin, end, transformedPoints, [transform](const auto& point){
    auto outPoint = transform.TransformPoint({point.x, point.y});
    return decltype(transformedPoints)::container_type::value_type ( outPoint.x, outPoint.y );
  });
}

void CreateConnectedLines(auto begin, auto end, auto lines, float x=0, float y=0, bool loop=true)
{
  std::transform(std::next(begin), end, begin, lines, [x, y](const auto& point2, const auto& point1)
  {
    return game_line { point1.x + x, point1.y + y, point2.x + x, point2.y + y };
  });

  if( loop )
  {
    auto last = std::prev(end);
    lines = game_line { last->x + x, last->y + y, begin->x + x, begin->y + y };
  }
};

#endif
