#include "pch.h"
#include "geometry.h"

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
