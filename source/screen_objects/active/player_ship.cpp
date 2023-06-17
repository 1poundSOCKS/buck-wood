#include "pch.h"
#include "player_ship.h"
#include "math.h"
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

auto player_ship::Update(int64_t tickCount) -> void
{
  auto updateInterval = framework::gameUpdateInterval(tickCount);

  if( m_state == state::alive )
  {
    const auto playerThrust = 400.0f;
    
    m_velocity.x -= ( ( m_velocity.x * 0.4f ) * updateInterval );
    m_velocity.y -= ( ( m_velocity.y * 0.4f ) * updateInterval );

    float forceX = m_thrusterOn ? playerThrust * sin(DEGTORAD(m_angle)) : 0.0f;
    float forceY = m_thrusterOn ? -playerThrust * cos(DEGTORAD(m_angle)) : 0.0f;

    m_velocity.x += forceX * updateInterval;
    m_velocity.y += forceY * updateInterval;

    m_x += m_velocity.x * updateInterval;
    m_y += m_velocity.y * updateInterval;

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
