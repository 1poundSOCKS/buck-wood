#include <d2d1.h>
#include <winrt/base.h>
#include "math.h"

struct game_object
{
  game_object() : size(20.0), xPos(0), yPos(0), xVelocity(0), yVelocity(0), angle(0) {}

  void Update(float seconds);
  void Accelerate(float);

  float size;
  float xPos, yPos;
  float xVelocity, yVelocity;
  float angle;
};

struct bullet
{
  bullet() : lifespanSeconds(0.5)
  {
    gameObject.size = 5.0;
  }

  void Update(float timespanSeconds);

  game_object gameObject;
  float lifespanSeconds;
};
