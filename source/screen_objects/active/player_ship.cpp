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

player_ship::player_ship() : 
  m_geometry { d2d_factory::get_raw(), GetPlayerGeometryData() }, 
  m_transformedGeometry { d2d_factory::get_raw(), m_geometry.Get(), D2D1::Matrix3x2F::Identity() }
{
  UpdateShipGeometryData();
}

player_ship::player_ship(const game_point& position) : 
  m_body { position }, m_geometry { d2d_factory::get_raw(), GetPlayerGeometryData() }, m_transformedGeometry(d2d_factory::get_raw(), m_geometry.Get(), D2D1::Matrix3x2F::Identity())
{
  UpdateShipGeometryData();
}

auto player_ship::Update(float interval) -> void
{
  if( m_state == state::alive )
  {
    m_previousState = m_body;
    m_body.Accelerate(m_thrust * interval);
    m_body.Update(interval);
    UpdateShipGeometryData();
  }
}

auto player_ship::UpdateShipGeometryData() -> void
{
  auto rotateAndMove = D2D1::Matrix3x2F::Rotation(m_body.Angle(), D2D1::Point2F(0, 0)) * D2D1::Matrix3x2F::Translation(m_body.Position().x, m_body.Position().y);
  m_transformedGeometry = { d2d_factory::get_raw(), m_geometry.Get(), rotateAndMove };
}

[[nodiscard]] auto player_ship::Geometry() const -> const transformed_path_geometry&
{
  return m_transformedGeometry;
}
