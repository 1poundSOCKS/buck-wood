#include "pch.h"
#include "player_ship.h"

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
