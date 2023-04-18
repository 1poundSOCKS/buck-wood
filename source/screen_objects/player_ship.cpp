#include "pch.h"
#include "player_ship.h"
#include "math.h"
#include "event_player_shot.h"
#include "event_player_dead.h"
#include "game_constants.h"

inline int shotTimeNumerator = 1;
inline int shotTimeDenominator = 20;

player_ship::player_ship(int64_t tickFrequency, screen_render_brush_selector brushes) : brushes(brushes), data(std::make_shared<data_type>())
{
  data->shotTimerInterval = ( tickFrequency * shotTimeNumerator ) / shotTimeDenominator;
  UpdateShipGeometryData();
  data->shipBrush.attach(brushes[white]);
  data->shipBrush->AddRef();
  data->thrusterBrush.attach(brushes[red]);
  data->thrusterBrush->AddRef();
}

auto player_ship::SetPosition(float x, float y) -> void
{
  data->xPos = x;
  data->yPos = y;
}

auto player_ship::SetThruster(bool thrusterOn) -> void
{
  data->controlState.thrust = thrusterOn;
}

auto player_ship::SetShoot(bool shoot) -> void
{
  data->controlState.shoot = shoot;  
}

auto player_ship::SetAngle(float angle) -> void
{
  data->angle = angle;
}

auto player_ship::SetEventShot(std::function<void(float,float,float)> eventShot) -> void
{
  data->eventShot = eventShot;
}

auto player_ship::SetEventDied(std::function<void(float,float)> eventDied) -> void
{
  data->eventDied = eventDied;
}

[[nodiscard]] auto player_ship::GetXPos() const -> float
{
  return data->xPos;
}

[[nodiscard]] auto player_ship::GetYPos() const -> float
{
  return data->yPos;
}

[[nodiscard]] auto player_ship::GetState() const -> state_type
{
  return data->state;
}

[[nodiscard]] auto player_ship::ThrusterOn() const -> bool
{
  return data->thrusterOn;
}

auto player_ship::Update(int64_t tickFrequency, int64_t tickCount, play_event_inserter playEventInserter) -> void
{
  if( data->state != player_ship::alive ) return;

  const auto forceOfGravity = 0.0f;
  const auto playerThrust = 200.0f;

  auto gameUpdateInterval = static_cast<float>(tickCount) / static_cast<float>(tickFrequency) * gameSpeedMultiplier;

  float forceX = 0.0f;
  float forceY = forceOfGravity;

  if( data->controlState.thrust )
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

  UpdateShipGeometryData();

  if( data->controlState.shoot && PlayerCanShoot(tickCount) )
  {
    playEventInserter = event_player_shot { data->xPos, data->yPos, data->angle, data->eventShot };
  }
}

[[nodiscard]] auto player_ship::LevelIsComplete() const -> bool
{
  return true;
}

auto player_ship::RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void
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

[[nodiscard]] auto player_ship::GetCollisionData() -> collision_data
{
  return m_collisionData;
}

[[nodiscard]] auto player_ship::HasCollidedWith(const collision_data& collisionData) const -> bool
{
  return collisionData.PointInside(data->xPos, data->yPos);
}

[[nodiscard]] auto player_ship::GetCollisionEffect() const -> collision_effect
{
  return {};
}

auto player_ship::ApplyCollisionEffect(const collision_effect& collisionEffect, play_event_inserter playEventInserter) -> void
{
  data->state = collisionEffect.GetProperty(collision_effect::kills_player) ? dead : alive;

  if( data->state == dead )
  {
    playEventInserter = event_player_dead { data->xPos, data->yPos, data->eventDied };
  }
}

[[nodiscard]] auto player_ship::Destroyed() const -> bool
{
  return false;
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
  CreateConnectedLines(data->points.cbegin(), data->points.cend(),std::back_inserter(lines));
  return lines;
}

[[nodiscard]] auto player_ship::PlayerCanShoot(int64_t tickCount) const -> bool
{
  data->shotTimer += tickCount;

  if( data->shotTimer >= data->shotTimerInterval )
  {
    data->shotTimer %= data->shotTimerInterval;
    return true;
  }
  else
    return false;
}