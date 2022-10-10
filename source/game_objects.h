#ifndef _game_object_
#define _game_object_

#include <memory>
#include <list>

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

struct player_ship
{
  player_ship();

  float xPos, yPos;
  float xVelocity, yVelocity;
  float angle;
  std::unique_ptr<game_shape> outline;
};

struct bullet
{
  bullet();

  float xPos, yPos;
  float xVelocity, yVelocity;
  float angle;
  float lifespanSeconds;
  bool outsideLevel;
};

std::unique_ptr<player_ship> CreatePlayerShip();
void InitializeShape(const game_point* points, int pointCount, game_shape& boundary);

#endif
