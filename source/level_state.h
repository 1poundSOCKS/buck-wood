#ifndef _level_state_
#define _level_state_

#include "game_level_data.h"

struct player_ship
{
  player_ship();

  float xPos, yPos;
  float xVelocity, yVelocity;
  float angle;
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

struct target_state
{
  target_state(const game_point& position);

  const game_point& position;
  std::vector<game_point> points;
  bool activated = false;
};

struct level_state
{
  level_state(const game_level_data& levelData);

  const game_level_data& levelData;
  player_ship player;
  std::list<std::unique_ptr<bullet>> bullets;
  std::vector<target_state> targets;
};

bool LevelIsComplete(const level_state& levelState);
void CreateRenderLines(const level_state& levelState, std::back_insert_iterator<std::vector<render_line>> renderLines);

#endif
