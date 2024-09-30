#include "pch.h"
#include "player_controls.h"

player_controls::player_controls(std::shared_ptr<player_ship_state> state) noexcept : m_state { state }
{
}

auto player_controls::SetState(std::shared_ptr<player_ship_state> state) noexcept -> void
{
  m_state = state;
}

auto player_controls::Update(float interval) const noexcept -> void
{
  auto leftThumbstickPosition = gamepad_reader::left_thumbstick();

  if( leftThumbstickPosition )
  {
    // constexpr float movementSpeedMultipier = 1000.0f;
    // auto position = m_state->Position();
    // auto positionOffset = POINT_2F { leftThumbstickPosition->x * movementSpeedMultipier * interval, leftThumbstickPosition->y * movementSpeedMultipier * interval };
    // auto newPosition = POINT_2F { position.x + positionOffset.x, position.y + positionOffset.y };
    // m_state->SetPosition(newPosition);

    constexpr float rotationSpeedMultiplier = 5.0f;
    m_state->RotateBy(leftThumbstickPosition->x * rotationSpeedMultiplier);
  }

  std::optional<D2D1_POINT_2F> rightThumbstickPosition = gamepad_reader::right_thumbstick();
  rightThumbstickPosition ? m_state->SetShootAngle(direct2d::GetAngleBetweenPoints({0,0}, *rightThumbstickPosition)) : m_state->ResetShootAngle();
}
