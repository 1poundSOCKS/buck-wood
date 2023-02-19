#include "pch.h"
#include "player_ship.h"
#include "math.h"

const float gameSpeedMultiplier = 2.0f;

auto player_ship::Update(int64_t tickFrequency, int64_t tickCount) -> void
{
  if( state != player_ship::alive ) return;

  const float forceOfGravity = 20.0f;
  const float playerThrust = 100.0f;

  float gameUpdateInterval = static_cast<float>(tickCount) / static_cast<float>(tickFrequency) * gameSpeedMultiplier;

  float forceX = 0.0f;
  float forceY = forceOfGravity;

  if( controlState->thrust )
  {
    thrusterOn = true;
    forceX += playerThrust * sin(DEGTORAD(angle));
    forceY -= playerThrust * cos(DEGTORAD(angle));
  }
  else
  {
    thrusterOn = false;
  }
  
  xVelocity += forceX * gameUpdateInterval;
  yVelocity += forceY * gameUpdateInterval;
  xPos += xVelocity * gameUpdateInterval;
  yPos += yVelocity * gameUpdateInterval;

  angle = CalculateAngle(xPos, yPos, controlState->mouseX, controlState->mouseY);

  UpdateShipGeometryData(*this);
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
