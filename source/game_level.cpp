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
  std::unique_ptr<game_level> level = std::make_unique<game_level>(levelWidth, levelHeight);
  const float boundarySize = 100.0f;

  const point points[] = {
    point(boundarySize, boundarySize),
    point(levelWidth - boundarySize, boundarySize),
    point(levelWidth - boundarySize, levelHeight - boundarySize),
    point(boundarySize, levelHeight - boundarySize)
  };

  const int pointCount = sizeof(points) / sizeof(point);
  
  InitializeShape(points, pointCount, level->boundary);

  return level;
}
