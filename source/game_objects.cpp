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

std::unique_ptr<game_object> CreatePlayerObject()
{
  std::unique_ptr<game_object> player = std::make_unique<game_object>();

  const point points[] = {
    point(0, -15),
    point(10, 15),
    point(-10, 15)
  };

  const int pointCount = sizeof(points) / sizeof(point);
  
  InitializeShape(points, pointCount, player->outline);

  return player;
}
