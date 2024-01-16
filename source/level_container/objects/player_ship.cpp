#include "pch.h"
#include "player_ship.h"

player_ship::player_ship(D2D1_POINT_2F position) : m_body { position, game_velocity { 0, 0 }, 0 }, m_previousState { m_body }
{
}

auto player_ship::Update(float interval) -> void
{
  if( m_state == state::alive )
  {
    m_previousState = m_body;

    diagnostics::add(L"Left thumb X", gamepad_reader::thumb_lx());
    diagnostics::add(L"Left thumb Y", gamepad_reader::thumb_ly());
    diagnostics::add(L"Left trigger", gamepad_reader::left_trigger());

    auto destinationDistance = direct2d::GetDistanceBetweenPoints(m_body.Position(), m_destination);
    SetThrust(destinationDistance > 10 ? destinationDistance / 400 : 0);

    auto playerAngle = destinationDistance > 10 ? direct2d::GetAngleBetween(m_body.Position(), m_destination) : m_body.Angle();
    m_body.SetAngle(playerAngle);
    
    m_body.Accelerate(m_thrust * interval);
    m_body.Update(interval);
  }
}
