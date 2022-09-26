#include "d2d_object.h"

D2D1_RECT_F d2d_object::GetRectangleForRender() const
{
  return D2D1::RectF(xPos + rectangle.left, yPos + rectangle.top, xPos + rectangle.right, yPos + rectangle.bottom);
}

void d2d_object::UpdatePosition()
{
  xPos += xVelocity;
  yPos += yVelocity;
}
