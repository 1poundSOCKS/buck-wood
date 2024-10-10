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
  m_state->SetRotationSpeed(leftThumbstickPosition ? leftThumbstickPosition->x : 0.0f);

  auto rightTrigger = gamepad_reader::right_trigger();
  m_state->SetThrusterPower(-rightTrigger);

  gamepad_reader::button_down(XINPUT_GAMEPAD_A) ? m_state->SetShootAngle(m_state->Angle()) : m_state->ResetShootAngle();  
}
