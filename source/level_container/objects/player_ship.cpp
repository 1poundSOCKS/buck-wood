#include "pch.h"
#include "player_ship.h"

player_ship::player_ship(movement_type movementType, POINT_2F position) : 
  base_object { position, { 1.8f, 1.8f }, 0 }, m_movementType { movementType }
{
}

auto player_ship::Update(float interval, bool enableControl) -> void
{
  enableControl && !m_destroyed ? UpdateWithControl(interval, enableControl) : UpdateWithoutControl(interval, enableControl);
}

auto player_ship::Update(float interval) -> void
{
  Update(interval, m_active);
}

auto player_ship::UpdateWithControl(float interval, bool enableControl) -> void
{
  switch( m_movementType )
  {
    case movement_type::horizontal:
      UpdateWithHorizontalControl(interval, enableControl);
      break;
    case movement_type::both:
      UpdateWithAllControl(interval, enableControl);
      break;
    default:
      UpdateWithAllControl(interval, enableControl);
      break;
  }
}

auto player_ship::UpdateWithoutControl(float interval, bool enableControl) -> void
{
  constexpr float rotationSpeed = 480.0f;
  auto rotationAmount = rotationSpeed * interval;
  m_angle = direct2d::RotateAngle(m_angle, rotationAmount);
}

auto player_ship::UpdateWithAllControl(float interval, bool enableControl) -> void
{
  std::optional<D2D1_POINT_2F> leftThumbstickPosition = gamepad_reader::left_thumbstick();
  m_destination = enableControl && leftThumbstickPosition ? std::optional<D2D1_POINT_2F>(direct2d::ShiftPosition(m_position, *leftThumbstickPosition)) : std::nullopt;

  auto thrustControlValue = enableControl ? gamepad_reader::left_trigger() : 0;
  auto shieldControlOn = enableControl ? gamepad_reader::button_down(XINPUT_GAMEPAD_A) : false;
  auto triggerControlOn = enableControl && gamepad_reader::right_trigger() > 0 ? true : false;
  auto switchFireMode = enableControl ? gamepad_reader::button_pressed(XINPUT_GAMEPAD_RIGHT_SHOULDER) : false;

  m_angle = m_destination ? GetUpdatedAngle(Position(), Angle(), *m_destination, interval) : m_angle;
  m_velocity = thrustControlValue > 0 ? direct2d::CombineVelocities(m_velocity, direct2d::CalculateVelocity(thrustControlValue * m_thrustPower * interval, m_angle)) : m_velocity;

  base_object::Update(interval);
  m_position = object_velocity::UpdatePosition(m_position, interval);

  m_thrusterOn = thrustControlValue > 0 ? true : false;
  m_shieldsUp = shieldControlOn;
  m_triggerDown = triggerControlOn;
  m_fireMode = switchFireMode ? SwitchFireMode(m_fireMode) : m_fireMode;

  m_playerReloadCounter.Update(interval);
  m_thrustEmmisionCounter.Update(interval);
}

auto player_ship::UpdateWithHorizontalControl(float interval, bool enableControl) -> void
{
  constexpr float movementSpeed = 1000.0f;
  std::optional<D2D1_POINT_2F> leftThumbstickPosition = gamepad_reader::left_thumbstick();
  m_velocity = enableControl && leftThumbstickPosition ? VELOCITY_2F { leftThumbstickPosition->x * movementSpeed, 0 } : VELOCITY_2F { 0, 0 };
  m_position = GetUpdatedPosition(m_position, m_velocity, interval);

  auto triggerControlOn = enableControl && gamepad_reader::right_trigger() > 0 ? true : false;
  m_triggerDown = triggerControlOn;

  m_thrusterOn = true;

  m_playerReloadCounter.Update(interval);
  m_thrustEmmisionCounter.Update(interval);
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
