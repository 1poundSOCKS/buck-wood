#ifndef _level_objects_
#define _level_objects_

#include "game_level_data.h"
#include "game_objects.h"

struct player_ship
{
  enum state_type { alive, dead };
  state_type state = alive;
  float xPos = 0;
  float yPos = 0;
  float xVelocity = 0;
  float yVelocity = 0;
  float angle = 0;
  bool thrusterOn = false;
  std::vector<game_point> transformedPoints;
};

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

void UpdateShipPointData(player_ship& playerShip);

void GetTransformedThrusterGeometry(const player_ship& ship, auto pointsInserter)
{
  const auto& thrusterGeometryData = GetPlayerThrusterGeometryData();

  TransformPoints(
    thrusterGeometryData.cbegin(), 
    thrusterGeometryData.cend(), 
    pointsInserter, 
    D2D1::Matrix3x2F::Rotation(ship.angle, D2D1::Point2F(0,0)) * D2D1::Matrix3x2F::Translation(ship.xPos, ship.yPos)
  );
}

void GetTransformedShipGeometry(const player_ship& ship, auto pointsInserter)
{
  const auto& shipGeometryData = GetPlayerGeometryData();

  TransformPoints(
    shipGeometryData.cbegin(), 
    shipGeometryData.cend(), 
    pointsInserter, 
    D2D1::Matrix3x2F::Rotation(ship.angle, D2D1::Point2F(0,0)) * D2D1::Matrix3x2F::Translation(ship.xPos, ship.yPos));
}

inline auto GenerateStar(std::uniform_int_distribution<int>& distX, std::uniform_int_distribution<int>& distY) -> level_star
{
  extern std::mt19937 rng;
  return { static_cast<float>(distX(rng)), static_cast<float>(distY(rng)) };
}

void GenerateStarfield(const game_level_data& levelData, auto starInserter)
{
  const float starDensity = 0.0005;
  auto levelBoundary = GetGameLevelBoundary(levelData);
  auto levelWidth = levelBoundary.bottomRight.x - levelBoundary.topLeft.x;
  auto levelHeight = levelBoundary.bottomRight.y - levelBoundary.topLeft.y;
  auto levelArea = levelWidth * levelHeight;
  int starCount = static_cast<int>(levelArea * starDensity);
  
  std::uniform_int_distribution<int> distX(levelBoundary.topLeft.x, levelBoundary.bottomRight.x);
  std::uniform_int_distribution<int> distY(levelBoundary.topLeft.y, levelBoundary.bottomRight.y);

  for( int starIndex = 0; starIndex < starCount; ++starIndex)
  {
    starInserter = GenerateStar(distX, distY);
  }
}

#endif
