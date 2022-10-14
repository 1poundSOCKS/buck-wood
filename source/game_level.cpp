#include "game_level.h"
#include "game_level_file.h"

game_level::game_level(float width, float height, std::unique_ptr<game_shape>& boundary, float playerStartX, float playerStartY)
 : width(width), height(height), boundary(std::move(boundary)), playerStartPosX(playerStartX), playerStartPosY(playerStartY)
{
}

bool game_level::OutOfBounds(float x, float y) const
{
  return ( x<0 || y<0 || x>width || y>height );
}

std::unique_ptr<game_level> CreateInitialGameLevel()
{
  game_level_file file;

  const float levelWidth = 2000.0f;
  const float levelHeight = 2200.0f;
  const float playerStartX = 1000.0f;
  const float playerStartY = 200.0f;
  
  const game_point boundaryPoints[] = {
    game_point(20, 20),
    game_point(1980, 20),
    game_point(1980, 980),
    game_point(1300, 980),
    game_point(1400, 2000),
    game_point(1300, 2100),
    game_point(1100, 2000),
    game_point(1200, 900),
    game_point(800, 700),
    game_point(20, 980)
  };

  const int boundaryPointCount = sizeof(boundaryPoints) / sizeof(game_point);

  std::unique_ptr<game_level> level = std::make_unique<game_level>
    (levelWidth, levelHeight, std::make_unique<game_shape>(boundaryPoints, boundaryPointCount), playerStartX, playerStartY);

  const game_point object1Points[] = {
    game_point(1600, 700),
    game_point(1700, 750),
    game_point(1700, 800),
    game_point(1750, 750),
    game_point(1700, 600)
  };

  level->objects.push_back(std::move(std::make_unique<game_shape>(object1Points, static_cast<int>(sizeof(object1Points) / sizeof(game_point)))));

  const game_point object2Points[] = {
    game_point(300, 200),
    game_point(400, 350),
    game_point(400, 600),
    game_point(350, 550),
    game_point(320, 400)
  };

  level->objects.push_back(std::move(std::make_unique<game_shape>(object2Points, static_cast<int>(sizeof(object2Points) / sizeof(game_point)))));

  level->targets.push_back(std::make_unique<target>(1250.0f, 1950.0f, 20.0f));
  level->targets.push_back(std::make_unique<target>(100.0f, 300.0f, 20.0f));
  level->targets.push_back(std::make_unique<target>(1800.0f, 900.0f, 20.0f));

  return level;
}

void ResetGameLevel(game_level& level)
{
  for( const auto& target: level.targets )
  {
    target->state = target::DEACTIVATED;
  }
}
