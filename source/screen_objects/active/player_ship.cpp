#include "pch.h"
#include "player_ship.h"
#include "math.h"
#include "event_player_shot.h"
#include "event_player_dead.h"
#include "perf_data.h"
#include "renderers.h"

player_ship::player_ship() : m_geometry(GetPlayerObject()), m_transformedGeometry(m_geometry, D2D1::Matrix3x2F::Identity())
{
  m_shotTimerInterval = GetShotTimeInterval();
  UpdateShipGeometryData();
}

player_ship::player_ship(float x, float y) : m_x(x), m_y(y), m_geometry(GetPlayerObject()), m_transformedGeometry(m_geometry, D2D1::Matrix3x2F::Identity())
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

[[nodiscard]] auto player_ship::State() const -> state_type
{
  return m_state;
}

[[nodiscard]] auto player_ship::ThrusterOn() const -> bool
{
  return m_thrusterOn;
}

[[nodiscard]] auto player_ship::Points() const -> const points_collection&
{
  return m_points;
}

auto player_ship::Update(const object_input_data& inputData, int64_t tickCount) -> void
{
  if( m_state == player_ship::alive )
  {
    auto thrusterOn = inputData.GetMouseData().rightButtonDown;
    auto triggerPressed = inputData.GetMouseData().leftButtonDown;
    auto angle = CalculateAngle(m_x, m_y, inputData.GetMouseData().x, inputData.GetMouseData().y);
    auto updateInterval = framework::gameUpdateInterval(tickCount);

    Update(thrusterOn, triggerPressed, angle, updateInterval);

    if( triggerPressed && PlayerCanShoot(tickCount) &&  m_eventShot )
    {
      m_eventShot(m_x, m_y, m_angle);
    }
  }
}

auto player_ship::Update(bool thrusterOn, bool triggerPressed, float angle, float gameUpdateInterval) -> void
{
  const auto playerThrust = 400.0f;
  
  m_thrusterOn = thrusterOn;
  m_angle = angle;

  m_velocityX -= ( ( m_velocityX * 0.4f ) * gameUpdateInterval );
  m_velocityY -= ( ( m_velocityY * 0.4f ) * gameUpdateInterval );

  float forceX = m_thrusterOn ? playerThrust * sin(DEGTORAD(m_angle)) : 0.0f;
  float forceY = m_thrusterOn ? -playerThrust * cos(DEGTORAD(m_angle)) : 0.0f;

  m_velocityX += forceX * gameUpdateInterval;
  m_velocityY += forceY * gameUpdateInterval;

  m_x += m_velocityX * gameUpdateInterval;
  m_y += m_velocityY * gameUpdateInterval;

  m_positionUpdate(m_x, m_y, m_thrusterOn);

  UpdateShipGeometryData();
}

[[nodiscard]] auto player_ship::GetCollisionData() const -> const collision_data&
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

  auto rotateAndMove = D2D1::Matrix3x2F::Rotation(m_angle, D2D1::Point2F(0, 0)) * D2D1::Matrix3x2F::Translation(m_x, m_y);
  m_transformedGeometry = { m_geometry, rotateAndMove };
}

auto player_ship::GetTransformedThrusterGeometry(std::back_insert_iterator<points_collection> pointsInserter) const -> void
{
  const auto& thrusterGeometryData = GetPlayerThrusterGeometryData();

  TransformPoints(thrusterGeometryData.cbegin(), thrusterGeometryData.cend(), pointsInserter, 
    D2D1::Matrix3x2F::Rotation(m_angle, D2D1::Point2F(0,0)) * D2D1::Matrix3x2F::Translation(m_x, m_y));
}

[[nodiscard]] auto player_ship::Geometry() const -> const transformed_path_geometry&
{
  return m_transformedGeometry;
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

game_closed_object player_ship::GetPlayerObject()
{
  const auto& geometryData = GetPlayerGeometryData();
  return { std::cbegin(geometryData), std::cend(geometryData) };
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
