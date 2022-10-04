#ifndef _game_level_
#define _game_level_

#include <list>
#include <memory>

struct point
{
  point() : x(0), y(0) {}
  point(float x, float y) : x(x), y(y) {}

  float x, y;
};

static const point end_point = point(-1, -1);

struct game_level
{
  game_level(float width, float height);
  bool OutOfBounds(float x, float y) const;

  const float width, height;
  const float playerStartPosX, playerStartPosY;
  std::list<std::pair<point, point>> boundary;
};

std::unique_ptr<game_level> GetInitialGameLevel();

#endif
