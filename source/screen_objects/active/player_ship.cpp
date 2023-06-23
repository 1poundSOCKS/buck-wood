#include "pch.h"
#include "player_ship.h"
#include "math.h"
#include "renderers.h"

constexpr auto GetPlayerGeometryData()
{
  return std::array {
    game_point { 0, -10 },
    game_point { 7, 10 },
    game_point { -7, 10 }
  };
}

constexpr auto GetPlayerThrusterGeometryData()
{
  return std::array {
    game_point { 5, 14 },
    game_point { -5, 14 }
  };
}

player_ship::player_ship() : m_geometry(GetPlayerGeometryData()), m_transformedGeometry(m_geometry, D2D1::Matrix3x2F::Identity())
{
  m_reloadTimer = reload_timer { GetShotTimeInterval() };
  UpdateShipGeometryData();
}

player_ship::player_ship(const game_point& position) : m_movingBody { position }, m_geometry { GetPlayerGeometryData() }, m_transformedGeometry(m_geometry, D2D1::Matrix3x2F::Identity())
{
  m_reloadTimer = reload_timer { GetShotTimeInterval() };
  UpdateShipGeometryData();
}

auto player_ship::Update(int64_t tickCount) -> void
{
  auto updateInterval = framework::gameUpdateInterval(tickCount);

  if( m_state == state::alive )
  {
    m_movingBody.Accelerate(-0.4f * updateInterval);
    m_movingBody.Accelerate(m_thrusterOn ? m_playerThrust * updateInterval : 0, m_angle);
    m_movingBody.Update(updateInterval);
    UpdateShipGeometryData();
  }

  m_reloaded = m_reloadTimer.Update(tickCount);
}

auto player_ship::UpdateShipGeometryData() -> void
{
  auto rotateAndMove = D2D1::Matrix3x2F::Rotation(m_angle, D2D1::Point2F(0, 0)) * D2D1::Matrix3x2F::Translation(m_movingBody.Position().x, m_movingBody.Position().y);
  m_transformedGeometry = { m_geometry, rotateAndMove };
}

auto player_ship::GetTransformedThrusterGeometry(std::back_insert_iterator<points_collection> pointsInserter) const -> void
{
  const auto& thrusterGeometryData = GetPlayerThrusterGeometryData();

  TransformPoints(thrusterGeometryData.cbegin(), thrusterGeometryData.cend(), pointsInserter, 
    D2D1::Matrix3x2F::Rotation(m_angle, D2D1::Point2F(0,0)) * D2D1::Matrix3x2F::Translation(m_movingBody.Position().x, m_movingBody.Position().y));
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
  return performance_counter::QueryFrequency() * m_shotTimeNumerator / m_shotTimeDenominator;
}
