#include "pch.h"
#include "player_ship.h"
#include "player_state.h"

player_ship::player_ship(POINT_2F position) : 
  base_object { position, { 1.8f, 1.8f }, 0 }
{
}

auto player_ship::Update(float interval) -> void
{
  switch( player_state::get_status() )
  {
    case player_state::status::active:
      UpdateWhenActive(interval);
      break;
    case player_state::status::celebrating:
      UpdateWhenCelebrating(interval);
      break;
  }
}

auto player_ship::UpdateWhenActive(float interval) -> void
{
  std::optional<D2D1_POINT_2F> leftThumbstickPosition = gamepad_reader::left_thumbstick();

  auto thrustControlValue = gamepad_reader::left_trigger();
  auto triggerControlOn = gamepad_reader::right_trigger() > 0 ? true : false;
  auto switchFireMode = gamepad_reader::button_pressed(XINPUT_GAMEPAD_RIGHT_SHOULDER);

  m_angle = leftThumbstickPosition ? direct2d::GetAngleBetweenPoints({0,0}, *leftThumbstickPosition) : m_angle;
  m_velocity = thrustControlValue > 0 ? direct2d::CombineVelocities(m_velocity, direct2d::CalculateVelocity(thrustControlValue * m_thrustPower * interval, m_angle)) : m_velocity;

  base_object::Update(interval);
  m_position = object_velocity::UpdatePosition(m_position, interval);

  m_thrusterOn = thrustControlValue > 0 ? true : false;
  m_triggerDown = triggerControlOn;
  m_fireMode = switchFireMode ? SwitchFireMode(m_fireMode) : m_fireMode;

  m_playerReloadCounter.Update(interval);
  m_thrustEmmisionCounter.Update(interval);
}

auto player_ship::UpdateWhenCelebrating(float interval) -> void
{
  constexpr float rotationSpeed = 480.0f;
  auto rotationAmount = rotationSpeed * interval;
  m_angle = direct2d::RotateAngle(m_angle, rotationAmount);
}

[[nodiscard]] auto player_ship::GetUpdatedAngle(D2D1_POINT_2F position, float direction, D2D1_POINT_2F destination, float interval) -> float
{
  constexpr float rotationSpeed = 360.0f;
  auto rotationAmount = rotationSpeed * interval;

  auto angleToDestination = direct2d::GetAngleBetweenPoints(position, destination);
  auto angleDifference = direct2d::GetAngleDifference(direction, angleToDestination);

  auto rotationAngle = angleDifference < 0 ? -rotationAmount : rotationAmount;
  auto updatedAngle = direction + rotationAngle;
  updatedAngle = updatedAngle < 0 ? updatedAngle + 360 : updatedAngle;
  return updatedAngle < 360 ? updatedAngle : updatedAngle - 360;
}

[[nodiscard]] auto player_ship::GetUpdatedPosition(D2D1_POINT_2F position, VELOCITY_2F velocity, float interval) -> D2D1_POINT_2F
{
  return { position.x + velocity.x * interval, position.y + velocity.y * interval };
}
