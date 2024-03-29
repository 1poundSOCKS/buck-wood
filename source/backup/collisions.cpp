#include "pch.h"
#include "collisions.h"

[[nodiscard]] auto GetBoundingRect(game_line line) -> game_rect
{
  return {
    min(line.start.x, line.end.x),
    min(line.start.y, line.end.y),
    max(line.start.x, line.end.x),
    max(line.start.y, line.end.y)
  };
}

[[nodiscard]] auto GetBoundingRect(game_rect rect1, game_rect rect2) -> game_rect
{
  return {
    min(rect1.topLeft.x, rect2.topLeft.x),
    min(rect1.topLeft.y, rect2.topLeft.y),
    max(rect1.bottomRight.x, rect2.bottomRight.x),
    max(rect1.bottomRight.y, rect2.bottomRight.y)
  };
}

game_rect::game_rect()
{
}

game_rect::game_rect(float left, float top, float right, float bottom) : topLeft { left, top}, bottomRight { right, bottom }
{
}

game_rect::game_rect(game_point topLeft, game_point bottomRight) : topLeft { topLeft }, bottomRight { bottomRight }
{
}

game_rect::game_rect(D2D1_RECT_F rect) : topLeft { rect.left, rect.top }, bottomRight { rect.right, rect.bottom }
{
}

[[nodiscard]] auto game_rect::Width() const -> float
{
  return bottomRight.x - topLeft.x;
}

[[nodiscard]] auto game_rect::Height() const -> float
{
  return bottomRight.y - topLeft.y;
}

[[nodiscard]] auto game_rect::CentrePoint() const -> game_point
{
  return { ( bottomRight.x + topLeft.x ) / 2, ( bottomRight.y + topLeft.y ) / 2 };
}

auto game_rect::Resize(float widthRatio, float heightRatio) -> void
{
  auto halfWidth = Width() * widthRatio / 2;
  auto halfHeight = Height() * heightRatio / 2;
  auto centrePoint = CentrePoint();
  topLeft = { centrePoint.x - halfWidth, centrePoint.y - halfHeight  };
  bottomRight = { centrePoint.x + halfWidth, centrePoint.y + halfHeight };
}

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

[[nodiscard]] auto game_closed_object::GetBoundingRect() const -> game_rect
{
  auto firstPoint = points.cbegin();
  return std::reduce(
    std::next(firstPoint), points.cend(), 
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

[[nodiscard]] bool DoOverlap(game_rect rect1, game_rect rect2) 
{
  return !(
    rect1.bottomRight.x < rect2.topLeft.x ||
    rect1.topLeft.x > rect2.bottomRight.x ||
    rect1.bottomRight.y < rect2.topLeft.y ||
    rect1.topLeft.y > rect2.bottomRight.y
  );
}

[[nodiscard]] auto GetCentrePoint(game_rect rect) -> game_point
{
  return {
    (rect.topLeft.x + rect.bottomRight.x) / 2,
    (rect.topLeft.y + rect.bottomRight.y) / 2
  };
}
