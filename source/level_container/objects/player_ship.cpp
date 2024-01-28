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

    auto angleToDestination = destinationDistance > deadzone ? direct2d::GetAngleBetweenPoints(m_body.Position(), *m_destination) : m_body.Angle();

    auto playerAngle = m_body.Angle();

    auto angleDifference = direct2d::GetAngleDifference(playerAngle, angleToDestination);

    constexpr float rotationSpeed = 3.0f;

    if( angleDifference >= -rotationSpeed && angleDifference <= rotationSpeed )
    {
      m_body.SetAngle(angleToDestination);
      auto distanceToGo = m_destination ? direct2d::GetDistanceBetweenPoints(m_body.Position(), *m_destination) : 0;

      if( distanceToGo < 5 )
      {
        m_body.SetPosition(m_destination ? *m_destination : m_body.Position());
      }
      else
      {
        m_body.Move(m_destination ? 5.0f : 0);
      }
    }
    else
    {
      auto directionalRotationSpeed = angleDifference < 0 ? -rotationSpeed : rotationSpeed;
      playerAngle = playerAngle + directionalRotationSpeed;
      playerAngle = playerAngle < 0 ? playerAngle + 360 : playerAngle;
      m_body.SetAngle(playerAngle);
    }
  }
}
