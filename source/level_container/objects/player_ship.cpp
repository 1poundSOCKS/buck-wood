#include "pch.h"
#include "player_ship.h"

player_ship::player_ship(D2D1_POINT_2F position)
{
}

auto player_ship::Update(float interval) -> void
{
  if( m_state == state::alive )
  {
    auto rightTriggerValue = gamepad_reader::right_trigger();
    m_previousPosition = m_position;
    m_angle = m_destination ? GetUpdatedAngle(Position(), Angle(), *m_destination) : m_angle;
    m_velocity = rightTriggerValue > 0 ? direct2d::CombineVelocities(m_velocity, direct2d::CalculateVelocity(rightTriggerValue * m_thrustPower * interval, m_angle)) : m_velocity;
    m_position = GetUpdatedPosition(Position(), m_velocity, interval);
    m_thrusterOn = rightTriggerValue ? true : false;
    m_triggerDown = gamepad_reader::button_down(XINPUT_GAMEPAD_A);
    m_shieldsUp = gamepad_reader::button_down(XINPUT_GAMEPAD_LEFT_SHOULDER);
  }
}

[[nodiscard]] auto player_ship::GetUpdatedAngle(D2D1_POINT_2F position, float direction, D2D1_POINT_2F destination) -> float
{
  constexpr float rotationSpeed = 3.0f;

  auto angleToDestination = direct2d::GetAngleBetweenPoints(position, destination);
  auto angleDifference = direct2d::GetAngleDifference(direction, angleToDestination);

  auto rotationAngle = angleDifference < 0 ? -rotationSpeed : rotationSpeed;
  auto updatedAngle = direction + rotationAngle;
  updatedAngle = updatedAngle < 0 ? updatedAngle + 360 : updatedAngle;
  return updatedAngle < 360 ? updatedAngle : updatedAngle - 360;
}

[[nodiscard]] auto player_ship::GetUpdatedPosition(D2D1_POINT_2F position, VELOCITY_2F velocity, float interval) -> D2D1_POINT_2F
{
  return { position.x + velocity.x * interval, position.y + velocity.y * interval };
}
