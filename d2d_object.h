#include <d2d1.h>
#include <winrt/base.h>
#include "math.h"

struct d2d_object
{
  d2d_object() : size(20.0), xPos(0), yPos(0), xVelocity(0), yVelocity(0), angle(0)
  {
  }

  D2D1_RECT_F GetRectangleForRender() const;
  void Update(double seconds);
  void Accelerate(double);

  double size;
  double xPos, yPos;
  double xVelocity, yVelocity;
  double angle;
};

struct bullet
{
  bullet() : lifespanSeconds(2)
  {
    d2dObject.size = 5.0;
  }

  void Update(double timespanSeconds);

  d2d_object d2dObject;
  double lifespanSeconds;
};
