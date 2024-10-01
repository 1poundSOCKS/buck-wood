#include "pch.h"
#include "player_ship_state.h"
#include "player_state.h"

auto player_ship_state::Update(VELOCITY_2F environmentalForces, float airResistance, float interval) -> void
{
  switch(  player_state::get_status() )
  {
    case player_state::status::active:
      UpdateWhenActive(environmentalForces, airResistance, interval);
      break;
    case  player_state::status::celebrating:
      UpdateWhenCelebrating(interval);
      break;
  }
}

auto player_ship_state::UpdateWhenActive(VELOCITY_2F environmentalForces, float airResistance, float interval) -> void
{
  base_object::Update(interval);
  VELOCITY_2F thrustVelocityChange = m_thrusterPower ? direct2d::CalculateVelocity(*m_thrusterPower, m_angle) : VELOCITY_2F { 0.0f, 0.0f };
  VELOCITY_2F invertedThrustVelocityChange = { -thrustVelocityChange.x, -thrustVelocityChange.y };
  VELOCITY_2F velocityChange = { invertedThrustVelocityChange.x + environmentalForces.x, invertedThrustVelocityChange.y + environmentalForces.y };
  m_velocity.Update(velocityChange, interval);
  m_velocity.RelativeUpdate(-airResistance, interval);
  m_position = m_velocity.UpdatePosition(m_position, interval);
  m_playerReloadCounter.Update(interval);
}

auto player_ship_state::UpdateWhenCelebrating(float interval) -> void
{
  constexpr float rotationSpeed = 480.0f;
  auto rotationAmount = rotationSpeed * interval;
  m_angle = direct2d::RotateAngle(m_angle, rotationAmount);
}
