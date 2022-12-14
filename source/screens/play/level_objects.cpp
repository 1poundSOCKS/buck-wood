#include "pch.h"
#include "level_objects.h"
#include "game_objects.h"

target_state::target_state(const game_point& position) : position(position)
{
  std::vector<game_point> pointsTmp;
  auto targetGeometryData = GetDefaultTargetGeometryData();
  TransformPoints(targetGeometryData.cbegin(), targetGeometryData.cend(), std::back_inserter(points), 0, position.x, position.y);
}

void UpdatePlayerShipPointData(player_ship& playerShip)
{
  playerShip.transformedPoints.clear();
  const auto& playerGeometryData = GetPlayerGeometryData();
  TransformPoints(
    playerGeometryData.cbegin(), 
    playerGeometryData.cend(), 
    std::back_inserter(playerShip.transformedPoints), 
    playerShip.angle, playerShip.xPos, playerShip.yPos);
}
