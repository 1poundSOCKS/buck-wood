#include "game_objects.h"

void game_object::Update(float seconds)
{
  xPos += xVelocity;
  yPos += yVelocity;
}

void game_object::Accelerate(float increase)
{
  yVelocity -= increase * cos(DEGTORAD(angle));
  xVelocity += increase * sin(DEGTORAD(angle));
}

void bullet::Update(float timespanSeconds)
{
  gameObject.Update(timespanSeconds);
  lifespanSeconds -= timespanSeconds;
}
