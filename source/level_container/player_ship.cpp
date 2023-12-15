#include "pch.h"
#include "player_ship.h"
#include "math.h"
#include "renderers.h"

player_ship::player_ship(const game_point& position) : 
  m_body { position }
{
}

auto player_ship::Update(float interval) -> void
{
  if( m_state == state::alive )
  {
    m_previousState = m_body;
    m_body.Accelerate(m_thrust * interval);
    m_body.Update(interval);
    auto rotateAndMove = D2D1::Matrix3x2F::Rotation(m_body.Angle(), D2D1::Point2F(0, 0)) * D2D1::Matrix3x2F::Translation(m_body.Position().x, m_body.Position().y);
    m_dg.Transform(rotateAndMove);
  }
}

[[nodiscard]] auto player_ship::Geometry() const -> const transformed_path_geometry&
{
  return m_dg;
}
