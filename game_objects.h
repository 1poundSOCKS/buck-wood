#include <d2d1.h>
#include <winrt/base.h>
#include "math.h"

struct game_object
{
  game_object() : size(20.0), xPos(0), yPos(0), xVelocity(0), yVelocity(0), angle(0) {}

  void Update(double seconds);
  void Accelerate(double);

  double size;
  double xPos, yPos;
  double xVelocity, yVelocity;
  double angle;
};

struct bullet
{
  bullet() : lifespanSeconds(0.5)
  {
    gameObject.size = 5.0;
  }

  void Update(double timespanSeconds);

  game_object gameObject;
  double lifespanSeconds;
};
