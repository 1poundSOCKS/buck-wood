#include "pch.h"
#include "level_objects.h"
#include "game_objects.h"

target_state::target_state(const game_point& position) : position(position)
{
  std::vector<game_point> pointsTmp;
  const auto& targetGeometryData = GetDefaultTargetGeometryData();
  TransformPoints(targetGeometryData.cbegin(), targetGeometryData.cend(), std::back_inserter(points), D2D1::Matrix3x2F::Translation(position.x, position.y));
}

void UpdatePlayerShipPointData(player_ship& playerShip)
{
  playerShip.transformedPoints.clear();
  const auto& playerGeometryData = GetPlayerGeometryData();
  TransformPoints(
    playerGeometryData.cbegin(), 
    playerGeometryData.cend(), 
    std::back_inserter(playerShip.transformedPoints), 
    CreateRotateAndTranslateTransform(playerShip.angle, playerShip.xPos, playerShip.yPos));
}
