#ifndef _game_level_
#define _game_level_

#include <list>
#include <memory>
#include "game_objects.h"

struct game_level
{
  game_level(float width, float height);
  bool OutOfBounds(float x, float y) const;

  const float width, height;
  const float playerStartPosX, playerStartPosY;
  game_shape boundary;
};

std::unique_ptr<game_level> CreateInitialGameLevel();

#endif
