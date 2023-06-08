#include "pch.h"
#include "player_ship.h"
#include "math.h"
#include "perf_data.h"
#include "renderers.h"

constexpr std::array<game_point, 3> GetPlayerGeometryData()
{
  return {
    game_point { 0, -10 },
    game_point { 7, 10 },
    game_point { -7, 10 }
  };
}

constexpr std::array<game_point, 2> GetPlayerThrusterGeometryData()
{
  return {
    game_point { 5, 14 },
    game_point { -5, 14 }
  };
}

player_ship::player_ship() : m_geometry(GetPlayerGeometryData()), m_transformedGeometry(m_geometry, D2D1::Matrix3x2F::Identity())
{
  m_reloadTimer = reload_timer { GetShotTimeInterval() };
  UpdateShipGeometryData();
}

player_ship::player_ship(float x, float y) : m_x(x), m_y(y), m_geometry(GetPlayerGeometryData()), m_transformedGeometry(m_geometry, D2D1::Matrix3x2F::Identity())
{
  m_reloadTimer = reload_timer { GetShotTimeInterval() };
  UpdateShipGeometryData();
}

auto player_ship::SetAngle(float angle) -> void
{
  m_angle = angle;
}

auto player_ship::SetThrusterOn(bool on) -> void
{
  m_thrusterOn = on;
}

auto player_ship::Destroy() -> void
{
  m_destroyed = true;
}

[[nodiscard]] auto player_ship::Position() const -> game_point
{
  return { m_x, m_y };
}

[[nodiscard]] auto player_ship::Angle() const -> float
{
  return m_angle;
}

[[nodiscard]] auto player_ship::State() const -> state
{
  return m_state;
}

[[nodiscard]] auto player_ship::ThrusterOn() const -> bool
{
  return m_thrusterOn;
}

[[nodiscard]] auto player_ship::Destroyed() const -> bool
{
  return m_destroyed;
}

auto player_ship::Update(int64_t tickCount) -> void
{
  auto updateInterval = framework::gameUpdateInterval(tickCount);

  if( m_state == state::alive )
  {
    const auto playerThrust = 400.0f;
    
    m_velocityX -= ( ( m_velocityX * 0.4f ) * updateInterval );
    m_velocityY -= ( ( m_velocityY * 0.4f ) * updateInterval );

    float forceX = m_thrusterOn ? playerThrust * sin(DEGTORAD(m_angle)) : 0.0f;
    float forceY = m_thrusterOn ? -playerThrust * cos(DEGTORAD(m_angle)) : 0.0f;

    m_velocityX += forceX * updateInterval;
    m_velocityY += forceY * updateInterval;

    m_x += m_velocityX * updateInterval;
    m_y += m_velocityY * updateInterval;

    UpdateShipGeometryData();
  }

  m_reloaded = m_reloadTimer.Update(tickCount);
}

auto player_ship::UpdateShipGeometryData() -> void
{
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

[[nodiscard]] auto player_ship::CanShoot() -> bool
{
  return m_reloaded;
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
