#ifndef _game_level_
#define _game_level_

#include <list>
#include <memory>
#include "game_objects.h"

struct game_level
{
  game_level(float width, float height, std::unique_ptr<game_shape>& boundary, float playerStartX, float playerStartY);
  bool OutOfBounds(float x, float y) const;

  const float width, height;
  const float playerStartPosX, playerStartPosY;
  std::unique_ptr<game_shape> boundary;
  std::list<std::unique_ptr<game_shape>> objects;
  std::unique_ptr<target> target;
};

std::unique_ptr<game_level> CreateInitialGameLevel();

#endif
