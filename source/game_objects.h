#ifndef _game_object_
#define _game_object_

#include <memory>
#include <vector>
#include <list>
#include <iterator>
#include "game_level_data.h"

struct game_line
{
  game_line(const game_point& start, const game_point& end);
  game_line(float startX, float startY, float endX, float endY);

  game_point start, end;
};

struct game_shape
{
  game_shape();
  game_shape(const game_point* points, int pointCount);
  game_shape(const std::vector<game_point>& pointsToCopy);
  game_shape(const game_level_object_data& objectData);

  std::list<game_point> points;
  std::list<game_line> lines;
};

struct mouse_cursor
{
  mouse_cursor();

  std::list<game_line> lines;
};

struct player_ship
{
  player_ship();

  float xPos, yPos;
  float xVelocity, yVelocity;
  float angle;
  std::unique_ptr<game_shape> outline;
  std::unique_ptr<game_line> thruster;
  bool thrusterOn = false;
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

struct target
{
  target(float x, float y, float size);

  enum STATE { DEACTIVATED, ACTIVATED };

  STATE state;
  game_shape shape;
};

struct game_level
{
  game_level(const game_level_data& gameLevelData);

  std::string name;
  float playerStartPosX = 0, playerStartPosY = 0;
  float timeLimitInSeconds = 0;
  std::unique_ptr<game_shape> boundary;
  std::list<std::unique_ptr<game_shape>> objects;
  std::list<std::unique_ptr<target>> targets;
};

std::unique_ptr<player_ship> CreatePlayerShip();

void InitializeShape(const game_point* points, int pointCount, game_shape& boundary);
void InitializeShape(std::list<game_point> points, game_shape& shape);

void CreateShapeLinesFromPoints(std::list<game_line>& lines, const std::list<game_point>& points);
void InitializeTargetShape(float x, float y, float size, game_shape& shape);

#endif
