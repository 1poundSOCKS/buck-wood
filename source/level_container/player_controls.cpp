#include "pch.h"
#include "player_controls.h"

auto player_controls::Update(player_ship_state& state, float interval) noexcept -> void
{
  auto leftThumbstickPosition = gamepad_reader::left_thumbstick();
  // state.SetRotationSpeed(leftThumbstickPosition ? leftThumbstickPosition->x : 0.0f);

  if( leftThumbstickPosition.has_value() )
  {
    auto leftThumbstickAngle = direct2d::GetAngle(leftThumbstickPosition.value());
    state.SetThrusterOn(100.0f, leftThumbstickAngle);
  }
  else
  {
    state.SetThrusterOff();
  }

  // auto rightTrigger = gamepad_reader::right_trigger();
  // state.SetThrusterPower(rightTrigger);

  gamepad_reader::button_down(XINPUT_GAMEPAD_A) ? state.SetShootAngle(state.Angle()) : state.ResetShootAngle();  
}
