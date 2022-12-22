#include "pch.h"
#include "level_objects.h"
#include "game_objects.h"

target_state::target_state(const game_point& position) : position(position)
{
  std::vector<game_point> pointsTmp;
  const auto& targetGeometryData = GetDefaultTargetGeometryData();
  TransformPoints(targetGeometryData.cbegin(), targetGeometryData.cend(), std::back_inserter(points), D2D1::Matrix3x2F::Translation(position.x, position.y));
}

void UpdateShipGeometryData(player_ship& ship)
{
  ship.points.clear();
  GetTransformedShipPointsGeometry(ship, std::back_inserter(ship.points));
  CreateConnectedLines(ship.points.cbegin(), ship.points.cend(), std::back_inserter(ship.lines));
}

[[nodiscard]] auto GetPlayerShipLineData(const player_ship& playerShip) -> std::vector<game_line>
{
  std::vector<game_line> lines;

  CreateConnectedLines(
    playerShip.points.begin(), 
    playerShip.points.cend(),
    std::back_inserter(lines));

  return lines;
}