#ifndef _level_objects_
#define _level_objects_

#include "game_level_data.h"

struct player_ship
{
  enum state_type { alive, dead };
  state_type state = alive;
  float xPos = 0;
  float yPos = 0;
  float xVelocity = 0;
  float yVelocity = 0;
  float angle = 0;
  bool thrusterOn = false;
};

struct player_ship_point_data
{
  std::vector<game_point> points;
  std::vector<game_point> transformedPoints;
  std::vector<game_point> thrusterPoints;
};

struct bullet
{
  bool free = true;
  float startX = 0, startY = 0;
  float xPos = 0, yPos = 0;
  float xVelocity = 0, yVelocity = 0;
  float angle = 0;
  float range = 1000;
};

struct target_state
{
  target_state(const game_point& position);
  const game_point& position;
  std::vector<game_point> points;
  bool activated = false;
  std::vector<game_line> shape;
};

struct bullet_target_collision
{
  bullet& bullet;
  target_state& targetState;
};

#endif
