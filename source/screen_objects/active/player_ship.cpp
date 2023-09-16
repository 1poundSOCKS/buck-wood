#include "pch.h"
#include "player_ship.h"
#include "math.h"
#include "renderers.h"

constexpr auto GetPlayerGeometryData()
{
  constexpr float width { 7 };
  constexpr float height { 10 };

  return std::array {
    game_point { 0, -height },
    game_point { width, height },
    game_point { -width, height }
  };
}

constexpr auto GetPlayerThrusterGeometryData()
{
  constexpr float width { 5 };
  constexpr float yOffset { 16 };
  
  return std::array {
    game_point { width, yOffset },
    game_point { -width, yOffset }
  };
}

player_ship::player_ship() : m_geometry(GetPlayerGeometryData()), m_transformedGeometry(m_geometry, D2D1::Matrix3x2F::Identity())
{
  UpdateShipGeometryData();
}

player_ship::player_ship(const game_point& position) : m_movingBody { position }, m_geometry { GetPlayerGeometryData() }, m_transformedGeometry(m_geometry, D2D1::Matrix3x2F::Identity())
{
  UpdateShipGeometryData();
}

auto player_ship::Update(float interval) -> void
{
  if( m_state == state::alive )
  {
    // m_movingBody.Accelerate(-1 * interval);
    m_movingBody.Accelerate(m_thrust * 20, m_angle);
    m_movingBody.Update(interval);
    UpdateShipGeometryData();
  }
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
