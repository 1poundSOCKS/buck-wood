#include "game_objects.h"

game_object::game_object() : game_object(NULL, 0)
{ 
}

game_object::game_object(const game_point* points, int pointCount) : size(20.0), xPos(0), yPos(0), xVelocity(0), yVelocity(0), angle(0), spin(0)
{
  outline = std::make_unique<game_shape>();
  InitializeShape(points, pointCount, *outline);
}

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
  const game_point points[] = {
    game_point(0, -15),
    game_point(10, 15),
    game_point(-10, 15)
  };

  const int pointCount = sizeof(points) / sizeof(game_point);

  return std::make_unique<game_object>(points, pointCount);
}

std::unique_ptr<game_object> CreateCursorObject()
{
  return std::make_unique<game_object>();
}

void InitializeShape(const game_point* points, int pointCount, game_shape& shape)
{
  for( int i = 0; i < pointCount; i++)
  {
    shape.points.push_back(points[i]);
    int endPointIndex = (i + 1) % pointCount;
    shape.lines.push_back(std::make_pair(points[i], points[endPointIndex]));
  }
}

bool ShapeInside(const game_shape& shape1, const game_shape& shape2)
{
  for( const auto& shape1Point: shape1.points )
  {
    shape1Point.x;
    shape1Point.y;
  }

  return true;
}
