#ifndef _game_level_
#define _game_level_

#include <list>
#include <memory>

struct game_level
{
  game_level(float width, float height);
  bool OutOfBounds(float x, float y) const;

  const float width, height;
  const float playerStartPosX, playerStartPosY;
};

std::unique_ptr<game_level> CreateGameLevel();

#endif
