#include <d2d1.h>
#include <winrt/base.h>

#define PI 3.14159265
#define DEGTORAD(D)((D * PI) / 180.0)
#define RADTODEG(R)((180.0 * R) / PI)

struct d2d_object
{
  d2d_object() : xPos(0), yPos(0), xVelocity(0), yVelocity(0), angle(0) {}
  D2D1_RECT_F GetRectangleForRender() const;
  void UpdatePosition();
  void Accelerate(double);

  double xPos, yPos;
  double xVelocity, yVelocity;
  double angle;
  D2D1_RECT_F rectangle = D2D1::RectF(-10, -10, 10, 10);
};
