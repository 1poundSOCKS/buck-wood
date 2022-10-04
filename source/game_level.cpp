#include "game_level.h"

game_level::game_level(float width, float height)
 : width(width), height(height), playerStartPosX(width / 2), playerStartPosY(height / 2)
{
}

bool game_level::OutOfBounds(float x, float y) const
{
  return ( x<0 || y<0 || x>width || y>height );
}

std::unique_ptr<game_level> GetInitialGameLevel()
{
  const float levelWidth = 2000.0f;
  const float levelHeight = 1000.0f;
  return std::make_unique<game_level>(levelWidth, levelHeight);
}
