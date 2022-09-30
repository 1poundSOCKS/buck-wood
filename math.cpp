#include "math.h"
#include <math.h>

int CalculateAngle(double x1, double y1, double x2, double y2)
{
  double radians = atan2(y2 - y1, x2 - x1);
  double degrees = RADTODEG(radians);
  return degrees + 90.5;
}
