#include "pch.h"
#include "player_ship.h"

player_ship::player_ship(POINT_2F position) : m_position { position }
{
}

auto player_ship::Update(float interval, bool enableControl) -> void
{
  m_destroyed ? UpdateWhenDestoyed(interval, enableControl) : UpdateWhenActive(interval, enableControl);
}

auto player_ship::Update(float interval) -> void
{
  Update(interval, m_active);
}

auto player_ship::UpdateWhenActive(float interval, bool enableControl) -> void
{
  std::optional<D2D1_POINT_2F> leftThumbstickPosition = gamepad_reader::left_thumbstick();
  m_destination = enableControl && leftThumbstickPosition ? std::optional<D2D1_POINT_2F>(direct2d::ShiftPosition(m_position, *leftThumbstickPosition)) : std::nullopt;

  auto thrustControlValue = enableControl ? gamepad_reader::left_trigger() : 0;
  auto shieldControlOn = enableControl ? gamepad_reader::button_down(XINPUT_GAMEPAD_A) : false;
  auto triggerControlOn = enableControl && gamepad_reader::right_trigger() > 0 ? true : false;
  auto switchFireMode = enableControl ? gamepad_reader::button_pressed(XINPUT_GAMEPAD_RIGHT_SHOULDER) : false;

  m_previousPosition = m_position;
  m_angle = m_destination ? GetUpdatedAngle(Position(), Angle(), *m_destination, interval) : m_angle;
  m_velocity = thrustControlValue > 0 ? direct2d::CombineVelocities(m_velocity, direct2d::CalculateVelocity(thrustControlValue * m_thrustPower * interval, m_angle)) : m_velocity;
  m_position = GetUpdatedPosition(m_position, m_velocity, interval);

  m_thrusterOn = thrustControlValue > 0 ? true : false;
  m_shieldsUp = shieldControlOn;
  m_triggerDown = triggerControlOn;
  m_fireMode = switchFireMode ? SwitchFireMode(m_fireMode) : m_fireMode;
  m_playerReloadCounter.Update(interval);
  m_thrustEmmisionCounter.Update(interval);
}

auto player_ship::UpdateWhenDestoyed(float interval, bool enableControl) -> void
{
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
