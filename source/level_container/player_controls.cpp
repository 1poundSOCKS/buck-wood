#include "pch.h"
#include "player_controls.h"

auto player_controls::Update(std::shared_ptr<player_ship_state> state, float interval) noexcept -> void
{
  auto leftThumbstickPosition = gamepad_reader::left_thumbstick();
  state->SetRotationSpeed(leftThumbstickPosition ? leftThumbstickPosition->x : 0.0f);

  auto rightTrigger = gamepad_reader::right_trigger();
  state->SetThrusterPower(-rightTrigger);

  gamepad_reader::button_down(XINPUT_GAMEPAD_A) ? state->SetShootAngle(state->Angle()) : state->ResetShootAngle();  
}
