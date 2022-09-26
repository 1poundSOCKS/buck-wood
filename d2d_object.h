#include <d2d1.h>
#include <winrt/base.h>

struct d2d_object
{
  d2d_object() : xPos(0), yPos(0), xVelocity(0), yVelocity(0), angle(0) {}
  D2D1_RECT_F GetRectangleForRender() const;
  void UpdatePosition();

  double xPos, yPos;
  double xVelocity, yVelocity;
  double angle;
  D2D1_RECT_F rectangle = D2D1::RectF(-10, -10, 10, 10);
};
