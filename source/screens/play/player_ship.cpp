#include "pch.h"
#include "player_ship.h"
#include "math.h"

const float gameSpeedMultiplier = 2.0f;

player_ship::player_ship(screen_render_brush_selector brushes)
{
  shipBrush.attach(brushes[white]);
  shipBrush->AddRef();
  thrusterBrush.attach(brushes[red]);
  thrusterBrush->AddRef();
}

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

void player_ship::RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const
{
  if( state != player_ship::alive ) return;

  std::vector<render_line> renderLines;
  auto renderLinesInserter = std::back_inserter(renderLines);

  CreateConnectedRenderLines(points.cbegin(), points.cend(), renderLinesInserter, shipBrush.get(), 2);

  if( thrusterOn )
  {
    std::vector<game_point> thrusterPoints;
    GetTransformedThrusterGeometry(*this, std::back_inserter(thrusterPoints));

    CreateDisconnectedRenderLines(thrusterPoints.cbegin(), thrusterPoints.cend(), renderLinesInserter, thrusterBrush.get(), 5);
  }

  RenderLines(renderTarget, renderLines.cbegin(), renderLines.cend());
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
