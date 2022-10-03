#include "game_objects.h"

void game_object::Update(float seconds)
{
  xPos += xVelocity * seconds;
  yPos += yVelocity * seconds;
  angle += spin * seconds;
}

void game_object::Accelerate(float amount)
{
  yVelocity -= amount * cos(DEGTORAD(angle));
  xVelocity += amount * sin(DEGTORAD(angle));
}

void bullet::Update(float timespanSeconds)
{
  gameObject.Update(timespanSeconds);
  lifespanSeconds -= timespanSeconds;
}
