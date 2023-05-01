#include "pch.h"
#include "player_ship.h"
#include "math.h"
#include "event_player_shot.h"
#include "event_player_dead.h"
#include "game_constants.h"
#include "render_brush_defs.h"
#include "perf_data.h"

player_ship::player_ship()
{
  m_shotTimerInterval = GetShotTimeInterval();
  UpdateShipGeometryData();
}

player_ship::player_ship(float x, float y) : m_x(x), m_y(y)
{
  m_shotTimerInterval = GetShotTimeInterval();
  UpdateShipGeometryData();
}

auto player_ship::SetPositionUpdate(position_update positionUpdate) -> void
{
  m_positionUpdate = positionUpdate;
  m_positionUpdate(m_x, m_y, m_thrusterOn);
}

auto player_ship::SetEventShot(std::function<void(float,float,float)> eventShot) -> void
{
  m_eventShot = eventShot;
}

auto player_ship::SetEventDied(std::function<void(float,float)> eventDied) -> void
{
  m_eventDied = eventDied;
}

auto player_ship::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_shipBrush = screen_render_brush_white.CreateBrush(renderTarget);
  m_thrusterBrush = screen_render_brush_red.CreateBrush(renderTarget);
}

auto player_ship::Update(const object_input_data& inputData, int64_t tickCount) -> void
{
  if( m_state == player_ship::alive && tickCount > 0 )
  {
    auto thrusterOn = inputData.GetMouseData().rightButtonDown;
    auto triggerPressed = inputData.GetMouseData().leftButtonDown;
    auto angle = CalculateAngle(m_x, m_y, inputData.GetMouseData().x, inputData.GetMouseData().y);

    Update(thrusterOn, triggerPressed, angle, tickCount);
  }
}

auto player_ship::Update(bool thrusterOn, bool triggerPressed, float angle, int64_t tickCount) -> void
{
  const auto forceOfGravity = 0.0f;
  const auto playerThrust = 200.0f;

  m_thrusterOn = thrusterOn;
  m_angle = angle;

  auto gameUpdateInterval = static_cast<float>(tickCount) / static_cast<float>(performance_counter::QueryFrequency()) * gameSpeedMultiplier;

  float forceX = 0.0f;
  float forceY = forceOfGravity;

  if( m_thrusterOn )
  {
    forceX += playerThrust * sin(DEGTORAD(m_angle));
    forceY -= playerThrust * cos(DEGTORAD(m_angle));
  }
  
  m_velocityX += forceX * gameUpdateInterval;
  m_velocityY += forceY * gameUpdateInterval;
  m_x += m_velocityX * gameUpdateInterval;
  m_y += m_velocityY * gameUpdateInterval;

  m_positionUpdate(m_x, m_y, m_thrusterOn);

  UpdateShipGeometryData();

  if( triggerPressed && PlayerCanShoot(tickCount) &&  m_eventShot )
  {
    m_eventShot(m_x, m_y, m_angle);
  }
}

auto player_ship::Render(D2D1_RECT_F viewRect) const -> void
{
  if( m_state != player_ship::alive ) return;

  std::vector<render_line> renderLines;
  auto renderLinesInserter = std::back_inserter(renderLines);

  CreateConnectedRenderLines(m_points.cbegin(), m_points.cend(), renderLinesInserter, m_shipBrush.get(), 3);

  if( m_thrusterOn )
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
  return collisionData.PointInside(m_x, m_y);
}

[[nodiscard]] auto player_ship::GetCollisionEffect() const -> collision_effect
{
  return {};
}

auto player_ship::ApplyCollisionEffect(const collision_effect& collisionEffect) -> void
{
  m_state = collisionEffect.GetProperty(collision_effect::kills_player) ? dead : alive;

  if( m_state == dead && m_eventDied )
  {
    m_eventDied(m_x, m_y);
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

auto player_ship::GetTransformedThrusterGeometry(std::back_insert_iterator<points_collection> pointsInserter) const -> void
{
  const auto& thrusterGeometryData = GetPlayerThrusterGeometryData();

  TransformPoints(thrusterGeometryData.cbegin(), thrusterGeometryData.cend(), pointsInserter, 
    D2D1::Matrix3x2F::Rotation(m_angle, D2D1::Point2F(0,0)) * D2D1::Matrix3x2F::Translation(m_x, m_y));
}

auto player_ship::GetTransformedShipPointsGeometry(std::back_insert_iterator<points_collection> pointsInserter) const -> void
{
  const auto& shipGeometryData = GetPlayerGeometryData();

  TransformPoints(shipGeometryData.cbegin(), shipGeometryData.cend(), pointsInserter, 
    D2D1::Matrix3x2F::Rotation(m_angle, D2D1::Point2F(0,0)) * D2D1::Matrix3x2F::Translation(m_x, m_y));
}

[[nodiscard]] auto player_ship::PlayerCanShoot(int64_t tickCount) -> bool
{
  m_shotTimer += tickCount;

  if( m_shotTimer < m_shotTimerInterval )
  {
    return false;
  }
  else
  {
    m_shotTimer %= m_shotTimerInterval;
    return true;
  }
}

auto player_ship::GetShotTimeInterval() -> int64_t
{
  return performance_counter::QueryFrequency() * GetShotTimeNumerator() / GetShotTimeDenominator();
}

constexpr auto player_ship::GetShotTimeNumerator() -> int64_t
{
  return 1;
}

constexpr auto player_ship::GetShotTimeDenominator() -> int64_t
{
  return 20;
}

constexpr std::array<game_point, 3> player_ship::GetPlayerGeometryData()
{
  return {
    game_point { 0, -10 },
    game_point { 7, 10 },
    game_point { -7, 10 }
  };
}

constexpr std::array<game_point, 2> player_ship::GetPlayerThrusterGeometryData()
{
  return {
    game_point { 5, 14 },
    game_point { -5, 14 }
  };
}
