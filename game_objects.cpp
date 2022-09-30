#include "game_objects.h"

void game_object::Update(double seconds)
{
  xPos += xVelocity;
  yPos += yVelocity;
}

void game_object::Accelerate(double increase)
{
  yVelocity -= increase * cos(DEGTORAD(angle));
  xVelocity += increase * sin(DEGTORAD(angle));
}

void bullet::Update(double timespanSeconds)
{
  gameObject.Update(timespanSeconds);
  lifespanSeconds -= timespanSeconds;
}
