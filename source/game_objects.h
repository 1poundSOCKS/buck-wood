#ifndef _game_object_
#define _game_object_

#include <d2d1.h>
#include <winrt/base.h>
#include "math.h"

struct game_point
{
  game_point() : game_point(0.0f, 0.0f) {}
  game_point(float x, float y) : x(x), y(y) {}

  float x, y;
};

typedef std::pair<game_point, game_point> game_line;

struct game_shape
{
  game_shape(const game_point* points, int pointCount);

  std::list<game_point> points;
  std::list<game_line> lines;
};

struct game_object
{
  game_object();
  game_object(const game_point* points, int pointCount);

  void Update(float seconds);
  void SetVelocity(float);

  float size;
  float xPos, yPos;
  float forceX, forceY;
  float xVelocity, yVelocity;
  float angle;
  float spin;
  std::unique_ptr<game_shape> outline;
};

struct bullet
{
  bullet() : lifespanSeconds(1.0), outsideLevel(false)
  {
    gameObject.size = 5.0;
  }

  void Update(float timespanSeconds);

  game_object gameObject;
  float lifespanSeconds;
  bool outsideLevel;
};

struct explosion
{
  game_object object;
};

std::unique_ptr<game_object> CreatePlayerObject();
std::unique_ptr<game_object> CreateCursorObject();
void InitializeShape(const game_point* points, int pointCount, game_shape& boundary);
bool PointsInside(const std::list<game_point>& points, const game_shape& shape);
bool PointInside(const std::list<game_point>& points, const game_shape& shape);
bool PointInside(const game_point& point, const game_shape& shape);
bool AddLineToInterceptCount(const game_line& line, const game_point& point);
float GetYIntercept(float x, const game_line& line);
void CalculateTransformedPoints(const game_object& gameObject, std::list<game_point>& transformedPoints);

#endif
