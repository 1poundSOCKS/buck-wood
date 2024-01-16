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

    m_body.SetAngle(direct2d::GetAngleBetween(m_body.Position(), m_destination));

    // auto rotation = gamepad_reader::thumb_lx() * 10.0f;
    // auto thrust = gamepad_reader::left_trigger();

    // auto realRotation = rotation ? rotation : 0;
    // m_body.Rotate(realRotation * interval * 20.0f);

    // SetThrust(thrust);
    SetThrust(direct2d::GetDistanceBetweenPoints(m_body.Position(), m_destination) / 200);

    m_body.Accelerate(m_thrust * interval);
    m_body.Update(interval);
  }
}
