#include "pch.h"
#include "player_ship.h"

player_ship::player_ship(D2D1_POINT_2F position) : m_body { position, { 0, 0 }, 0 }, m_previousState { m_body }
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

    constexpr float deadzone = 100.0f;

    auto destinationDistance = m_destination ? direct2d::GetDistanceBetweenPoints(m_body.Position(), *m_destination) : 0;
    SetThrust(destinationDistance > deadzone ? ( destinationDistance - deadzone ) / 400 : 0);

    auto playerAngle = destinationDistance > deadzone ? direct2d::GetAngleBetween(m_body.Position(), *m_destination) : m_body.Angle();

    m_body.SetAngle(playerAngle);
    m_body.Accelerate(m_thrust ? m_thrust * interval : -0.01f);
    m_body.Update(interval);
  }
}
