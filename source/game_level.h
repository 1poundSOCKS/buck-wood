#include <list>

#ifndef _game_level_
#define _game_level_

struct game_level
{
  game_level(float width, float height, std::list<float> boundary);

  const float width, height;
  const std::list<float> boundary;
  const float playerStartPosX, playerStartPosY;
};

#endif
