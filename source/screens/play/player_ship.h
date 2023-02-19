#ifndef _player_ship_
#define _player_ship_

#include "game_objects.h"
#include "player_control_state.h"

struct player_ship
{
  auto Update(int64_t tickFrequency, int64_t tickCount) -> void;

  enum state_type { alive, dead };
  state_type state = alive;
  float xPos = 0;
  float yPos = 0;
  float xVelocity = 0;
  float yVelocity = 0;
  float angle = 0;
  bool thrusterOn = false;
  std::vector<game_point> points;
  std::vector<game_line> lines;
  std::shared_ptr<player_control_state> controlState;
};

void UpdateShipGeometryData(player_ship& playerShip);

[[nodiscard]] auto GetPlayerShipLineData(const player_ship& playerShip) -> std::vector<game_line>;

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

void GetTransformedShipPointsGeometry(const player_ship& ship, auto pointsInserter)
{
  const auto& shipGeometryData = GetPlayerGeometryData();

  TransformPoints(
    shipGeometryData.cbegin(), 
    shipGeometryData.cend(), 
    pointsInserter, 
    D2D1::Matrix3x2F::Rotation(ship.angle, D2D1::Point2F(0,0)) * D2D1::Matrix3x2F::Translation(ship.xPos, ship.yPos));
}

#endif
