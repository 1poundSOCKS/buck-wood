#include "pch.h"
#include "player_ship_state.h"

auto player_ship_state::Update(VELOCITY_2F environmentalForces, float airResistance, float interval, bool levelComplete) -> void
{
  levelComplete ? UpdateWhenCelebrating(interval) : UpdateWhenActive(environmentalForces, airResistance, interval);
}

auto player_ship_state::UpdateWhenActive(VELOCITY_2F environmentalForces, float airResistance, float interval) -> void
{
  base_object::Update(interval);

  RotateBy(m_rotationSpeed, interval);

  VELOCITY_2F thrustVelocityChange = direct2d::CalculateVelocity(ThrusterPower(), m_angle);
  VELOCITY_2F velocityChange = { thrustVelocityChange.x + environmentalForces.x, thrustVelocityChange.y + environmentalForces.y };

  m_velocity.Update(velocityChange, interval);
  m_velocity.RelativeUpdate(-airResistance, interval);

  m_position = m_velocity.UpdatePosition(m_position, interval);

  m_playerReloadCounter.Update(interval);
}

auto player_ship_state::UpdateWhenCelebrating(float interval) -> void
{
  constexpr float rotationSpeed = 450.0f;
  auto rotationAmount = rotationSpeed * interval;
  m_angle = direct2d::RotateAngle(m_angle, rotationAmount);
}
