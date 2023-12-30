#include "pch.h"
#include "player_ship.h"

player_ship::player_ship(game_point position) : m_body { position, game_velocity { 0, 0 }, 0 }, m_previousState { m_body }
{
}

auto player_ship::Update(float interval) -> void
{
  if( m_state == state::alive )
  {
    diagnostics::add(L"Left thumb X", gamepad_reader::thumb_lx());
    diagnostics::add(L"Left thumb Y", gamepad_reader::thumb_ly());
    diagnostics::add(L"Left trigger", gamepad_reader::left_trigger());

    auto rotation = gamepad_reader::thumb_lx() * 10.0f;
    auto thrust = gamepad_reader::left_trigger();

    m_previousState = m_body;

    auto realRotation = rotation ? rotation : 0;
    m_body.Rotate(realRotation * interval * 20.0f);

    SetThrust(thrust);

    m_body.Accelerate(m_thrust * interval);
    m_body.Update(interval);
  }
}
