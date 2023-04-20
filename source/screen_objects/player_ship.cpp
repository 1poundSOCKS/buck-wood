#include "pch.h"
#include "player_ship.h"
#include "math.h"
#include "event_player_shot.h"
#include "event_player_dead.h"
#include "game_constants.h"
#include "render_defs.h"
#include "clock_frequency.h"

inline int shotTimeNumerator = 1;
inline int shotTimeDenominator = 20;

auto player_ship::control::SetPosition(float x, float y) -> void
{
  m_x = x;
  m_y = y;
}

auto player_ship::control::SetAngle(float angle) -> void
{
  m_angle = angle;
}

auto player_ship::control::SetThruster(bool thrusterOn) -> void
{
  m_thrusterOn = thrusterOn;
}

auto player_ship::control::SetTrigger(bool triggerPressed) -> void
{
  m_triggerPressed = triggerPressed;
}

auto player_ship::control::SetEventShot(std::function<void(float,float,float)> eventShot) -> void
{
  m_eventShot = eventShot;
}

auto player_ship::control::SetEventDied(std::function<void(float,float)> eventDied) -> void
{
  m_eventDied = eventDied;
}

[[nodiscard]] auto player_ship::control::GetPosition() const -> game_point
{
  return { m_x, m_y };
}

[[nodiscard]] auto player_ship::control::ThrusterOn() const -> bool
{
  return m_thrusterOn;
}

player_ship::player_ship(control_data controlData) : m_controlData(controlData)
{
  m_shotTimerInterval = ( clock_frequency::get() * shotTimeNumerator ) / shotTimeDenominator;
  UpdateShipGeometryData();
}

auto player_ship::Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_shipBrush = screen_render_brush_white.CreateBrush(renderTarget);
  m_thrusterBrush = screen_render_brush_red.CreateBrush(renderTarget);
}

auto player_ship::Update(int64_t tickCount, play_event_inserter playEventInserter) -> void
{
  if( m_state != player_ship::alive ) return;

  const auto forceOfGravity = 0.0f;
  const auto playerThrust = 200.0f;

  auto gameUpdateInterval = static_cast<float>(tickCount) / static_cast<float>(performance_counter::QueryFrequency()) * gameSpeedMultiplier;

  float forceX = 0.0f;
  float forceY = forceOfGravity;

  if( m_controlData->m_thrusterOn )
  {
    forceX += playerThrust * sin(DEGTORAD(m_controlData->m_angle));
    forceY -= playerThrust * cos(DEGTORAD(m_controlData->m_angle));
  }
  
  m_velocityX += forceX * gameUpdateInterval;
  m_velocityY += forceY * gameUpdateInterval;
  m_controlData->m_x += m_velocityX * gameUpdateInterval;
  m_controlData->m_y += m_velocityY * gameUpdateInterval;

  UpdateShipGeometryData();

  if( m_controlData->m_triggerPressed && PlayerCanShoot(tickCount) )
  {
    playEventInserter = event_player_shot { m_controlData->m_x, m_controlData->m_y, m_controlData->m_angle, m_controlData->m_eventShot };
  }
}

[[nodiscard]] auto player_ship::LevelIsComplete() const -> bool
{
  return true;
}

auto player_ship::Render(D2D1_RECT_F viewRect) const -> void
{
  if( m_state != player_ship::alive ) return;

  std::vector<render_line> renderLines;
  auto renderLinesInserter = std::back_inserter(renderLines);

  CreateConnectedRenderLines(m_points.cbegin(), m_points.cend(), renderLinesInserter, m_shipBrush.get(), 2);

  if( m_controlData->m_thrusterOn )
  {
    std::vector<game_point> thrusterPoints;
    GetTransformedThrusterGeometry(std::back_inserter(thrusterPoints));
    CreateDisconnectedRenderLines(thrusterPoints.cbegin(), thrusterPoints.cend(), renderLinesInserter, m_thrusterBrush.get(), 5);
  }

  RenderLines(m_renderTarget.get(), renderLines.cbegin(), renderLines.cend());
}

[[nodiscard]] auto player_ship::GetCollisionData() -> collision_data
{
  return m_collisionData;
}

[[nodiscard]] auto player_ship::HasCollidedWith(const collision_data& collisionData) const -> bool
{
  return collisionData.PointInside(m_controlData->m_x, m_controlData->m_y);
}

[[nodiscard]] auto player_ship::GetCollisionEffect() const -> collision_effect
{
  return {};
}

auto player_ship::ApplyCollisionEffect(const collision_effect& collisionEffect, play_event_inserter playEventInserter) -> void
{
  m_state = collisionEffect.GetProperty(collision_effect::kills_player) ? dead : alive;

  if( m_state == dead )
  {
    playEventInserter = event_player_dead { m_controlData->m_x, m_controlData->m_y, m_controlData->m_eventDied };
  }
}

[[nodiscard]] auto player_ship::Destroyed() const -> bool
{
  return false;
}

auto player_ship::UpdateShipGeometryData() -> void
{
  m_points.clear();
  m_lines.clear();
  GetTransformedShipPointsGeometry(std::back_inserter(m_points));
  CreateConnectedLines(m_points.cbegin(), m_points.cend(), std::back_inserter(m_lines));
}

[[nodiscard]] auto player_ship::GetPlayerShipLineData() const -> std::vector<game_line>
{
  std::vector<game_line> lines;
  CreateConnectedLines(m_points.cbegin(), m_points.cend(),std::back_inserter(lines));
  return lines;
}

[[nodiscard]] auto player_ship::PlayerCanShoot(int64_t tickCount) -> bool
{
  m_shotTimer += tickCount;

  if( m_shotTimer >= m_shotTimerInterval )
  {
    m_shotTimer %= m_shotTimerInterval;
    return true;
  }
  else
    return false;
}
