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

struct game_line
{
  game_line(float startX, float startY, float endX, float endY);

  game_point start, end;
};

struct game_shape
{
  game_shape(const game_point* points, int pointCount);

  std::list<game_point> points;
  std::list<game_line> lines;
};

struct mouse_cursor
{
  mouse_cursor();

  float xPos, yPos;
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
  bullet(float x, float y, float range);

  float startX, startY;
  float xPos, yPos;
  float xVelocity, yVelocity;
  float angle;
  float range;
  bool outsideLevel;
};

std::unique_ptr<player_ship> CreatePlayerShip();
void InitializeShape(const game_point* points, int pointCount, game_shape& boundary);

#endif
