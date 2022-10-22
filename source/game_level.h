#ifndef _game_level_
#define _game_level_

#include <vector>
#include <list>
#include <memory>
#include "game_objects.h"

using game_level_object_data = std::vector<game_point>;
using game_level_object_data_ptr = std::unique_ptr<game_level_object_data>;

struct game_level_data
{
  float width = 0, height = 0;
  float playerStartPosX = 0, playerStartPosY = 0;
  int timeLimitInSeconds = 0;
  std::vector<game_point> boundaryPoints;
  std::vector<game_level_object_data_ptr> objects;
  std::vector<game_point> targets;
};

using game_level_data_ptr = std::shared_ptr<game_level_data>;

struct game_level
{
  game_level(const game_level_data& gameLevelData);
 
  float width = 0, height = 0;
  float playerStartPosX = 0, playerStartPosY = 0;
  float timeLimitInSeconds = 0;
  std::unique_ptr<game_shape> boundary;
  std::list<std::unique_ptr<game_shape>> objects;
  std::list<std::unique_ptr<target>> targets;
};

using game_level_ptr = std::shared_ptr<game_level>;

bool OutOfGameLevelBoundary(const game_level& gameLevel, float x, float y);

game_level_data_ptr CreateFirstGameLevelData();
game_level_data_ptr CreateSecondGameLevelData();
game_level_data_ptr CreateThirdGameLevelData();

#endif
