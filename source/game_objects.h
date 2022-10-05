#ifndef _game_object_
#define _game_object_

#include <d2d1.h>
#include <winrt/base.h>
#include "math.h"

struct game_point
{
  game_point() : x(0), y(0) {}
  game_point(float x, float y) : x(x), y(y) {}

  float x, y;
};

typedef std::pair<game_point, game_point> game_line;

struct game_shape
{
  std::list<game_point> points;
  std::list<game_line> lines;
};

struct game_object
{
  game_object();
  game_object(const game_point* points, int pointCount);

  void Update(float seconds);
  void Accelerate(float);

  float size;
  float xPos, yPos;
  float xVelocity, yVelocity;
  float angle;
  float spin;
  std::unique_ptr<game_shape> outline;
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
std::unique_ptr<game_object> CreateCursorObject();
void InitializeShape(const game_point* points, int pointCount, game_shape& boundary);
bool ShapeInside(const game_shape& shape1, const game_shape& shape2);

#endif
