#ifndef _game_object_
#define _game_object_

#include <d2d1.h>
#include <winrt/base.h>
#include "math.h"

struct point
{
  point() : x(0), y(0) {}
  point(float x, float y) : x(x), y(y) {}

  float x, y;
};

typedef std::list<std::pair<point, point>> shape;

struct game_object
{
  game_object() : size(20.0), xPos(0), yPos(0), xVelocity(0), yVelocity(0), angle(0), spin(0) {}

  void Update(float seconds);
  void Accelerate(float);

  float size;
  float xPos, yPos;
  float xVelocity, yVelocity;
  float angle;
  float spin;
  shape outline;
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

std::unique_ptr<game_object> CreatePlayerObject();
void InitializeShape(const point* points, int pointCount, shape& boundary);

#endif
