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
    m_body.SetAngle(m_destination ? GetUpdatedAngle(Position(), Angle(), *m_destination) : m_body.Angle());
    m_body.SetVelocity(m_destination ? direct2d::CombineVelocities(m_body.Velocity(), direct2d::CalculateVelocity(1, m_body.Angle())) : m_body.Velocity());
    m_body.SetPosition(GetUpdatedPosition(Position(), m_body.Velocity(), m_body.Angle(), interval));
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

[[nodiscard]] auto player_ship::GetUpdatedPosition(D2D1_POINT_2F position, VELOCITY_2F velocity, float direction, float interval) -> D2D1_POINT_2F
{
  return { position.x + velocity.x * interval, position.y + velocity.y * interval };
}
