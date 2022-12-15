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

struct bullet_target_collision
{
  bullet& bullet;
  target_state& targetState;
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

#endif
