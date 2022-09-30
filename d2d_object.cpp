#include "d2d_object.h"

D2D1_RECT_F d2d_object::GetRectangleForRender() const
{
  return D2D1::RectF(xPos - size / 2, yPos - size / 2, xPos + size / 2, yPos + size / 2);
}

void d2d_object::Update(double seconds)
{
  xPos += xVelocity;
  yPos += yVelocity;
}

void d2d_object::Accelerate(double increase)
{
  yVelocity -= increase * cos(DEGTORAD(angle));
  xVelocity += increase * sin(DEGTORAD(angle));
}

void bullet::Update(double timespanSeconds)
{
  d2dObject.Update(timespanSeconds);
  lifespanSeconds -= timespanSeconds;
}
