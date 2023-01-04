#ifndef _level_objects_
#define _level_objects_

#include "game_level_data.h"
#include "game_objects.h"

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

struct level_star
{
  game_point position;
};

struct level_starfield
{
  std::vector<level_star> stars;
};

struct level_background_data
{
  level_starfield starfield;
};

inline auto GenerateStar(std::uniform_int_distribution<int> distX, std::uniform_int_distribution<int> distY) -> level_star
{
  extern std::mt19937 rng;
  return { static_cast<float>(distX(rng)), static_cast<float>(distY(rng)) };
}

void GenerateStarfield(const game_level_data& levelData, auto starInserter)
{
  const float starDensity = 0.001;
  auto levelBoundary = GetGameLevelBoundary(levelData);
  auto levelWidth = levelBoundary.bottomRight.x - levelBoundary.topLeft.x;
  auto levelHeight = levelBoundary.bottomRight.y - levelBoundary.topLeft.y;
  auto levelArea = levelWidth * levelHeight;
  auto starCount = static_cast<int>(levelArea * starDensity);
  
  std::uniform_int_distribution<int> distX(levelBoundary.topLeft.x, levelBoundary.bottomRight.x);
  std::uniform_int_distribution<int> distY(levelBoundary.topLeft.y, levelBoundary.bottomRight.y);

  for( int starIndex = 0; starIndex < starCount; ++starIndex)
  {
    starInserter = GenerateStar(distX, distY);
  }
}

#endif
