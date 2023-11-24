#include "pch.h"
#include "player_ship_renderer.h"

player_ship_renderer::player_ship_renderer()
{
  constexpr float width { 5 };
  constexpr float top { 14 };
  constexpr float bottom { 20 };

  m_thrusterGeometry.Load(std::initializer_list { game_point { width, top }, game_point { -width, top },
    game_point { -width, bottom }, game_point { width, bottom } });
}

auto player_ship_renderer::Write(const player_ship& playerShip) const -> void
{
  m_renderer.Write(playerShip.Geometry());

  if( playerShip.ThrusterOn() )
  {
    const auto& position = playerShip.Position();
    auto thrusterTransform = D2D1::Matrix3x2F::Rotation(playerShip.Angle(), D2D1::Point2F(0,0)) * D2D1::Matrix3x2F::Translation(position.x, position.y);
    transformed_path_geometry transforedThrusterGeometry { d2d_factory::get_raw(), m_thrusterGeometry.Get(), thrusterTransform };
    m_thrusterRenderer.Write(transforedThrusterGeometry);
  }
}
