#include "pch.h"
#include "player_ship.h"
#include "math.h"
#include "event_player_shot.h"

const float gameSpeedMultiplier = 2.0f;
const int shotTimeNumerator = 1;
const int shotTimeDenominator = 20;

player_ship::player_ship(screen_render_brush_selector brushes) : data(std::make_shared<data_type>())
{
  UpdateShipGeometryData();
  data->shipBrush.attach(brushes[white]);
  data->shipBrush->AddRef();
  data->thrusterBrush.attach(brushes[red]);
  data->thrusterBrush->AddRef();
}

auto player_ship::Update(int64_t tickFrequency, int64_t tickCount, play_event_inserter playEventInserter) -> void
{
  if( data->state != player_ship::alive ) return;

  const float forceOfGravity = 20.0f;
  const float playerThrust = 100.0f;

  float gameUpdateInterval = static_cast<float>(tickCount) / static_cast<float>(tickFrequency) * gameSpeedMultiplier;

  float forceX = 0.0f;
  float forceY = forceOfGravity;

  if( data->controlState->thrust )
  {
    data->thrusterOn = true;
    forceX += playerThrust * sin(DEGTORAD(data->angle));
    forceY -= playerThrust * cos(DEGTORAD(data->angle));
  }
  else
  {
    data->thrusterOn = false;
  }
  
  data->xVelocity += forceX * gameUpdateInterval;
  data->yVelocity += forceY * gameUpdateInterval;
  data->xPos += data->xVelocity * gameUpdateInterval;
  data->yPos += data->yVelocity * gameUpdateInterval;

  data->angle = CalculateAngle(data->xPos, data->yPos, data->controlState->mouseX, data->controlState->mouseY);

  UpdateShipGeometryData();

  if( data->controlState->shoot && PlayerCanShoot(tickCount) )
    playEventInserter = event_player_shot { data->xPos, data->yPos, data->angle, data->eventShot };
}

[[nodiscard]] auto player_ship::HasCollided(float x, float y) const -> bool
{
  return false;
}

auto player_ship::HitByBullet() -> void
{
}

[[nodiscard]] auto player_ship::LevelIsComplete() const -> bool
{
  return true;
}

void player_ship::RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const
{
  if( data->state != player_ship::alive ) return;

  std::vector<render_line> renderLines;
  auto renderLinesInserter = std::back_inserter(renderLines);

  CreateConnectedRenderLines(data->points.cbegin(), data->points.cend(), renderLinesInserter, data->shipBrush.get(), 2);

  if( data->thrusterOn )
  {
    std::vector<game_point> thrusterPoints;
    GetTransformedThrusterGeometry(std::back_inserter(thrusterPoints));
    CreateDisconnectedRenderLines(thrusterPoints.cbegin(), thrusterPoints.cend(), renderLinesInserter, data->thrusterBrush.get(), 5);
  }

  RenderLines(renderTarget, renderLines.cbegin(), renderLines.cend());
}

auto player_ship::UpdateShipGeometryData() -> void
{
  data->points.clear();
  GetTransformedShipPointsGeometry(std::back_inserter(data->points));
  CreateConnectedLines(data->points.cbegin(), data->points.cend(), std::back_inserter(data->lines));
}

[[nodiscard]] auto player_ship::GetPlayerShipLineData() const -> std::vector<game_line>
{
  std::vector<game_line> lines;

  CreateConnectedLines(
    data->points.cbegin(), 
    data->points.cend(),
    std::back_inserter(lines));

  return lines;
}

[[nodiscard]] auto player_ship::PlayerCanShoot(int64_t tickCount) const -> bool
{
  return data->controlState->shoot;
//  return currentTimerCount - lastShotTimerValue > shotTimerInterval;
}
