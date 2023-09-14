#pragma once

#include "main_window.h"
#include "gamepad_thumbstick.h"
#include "gamepad_buttons.h"

class object_input_data
{
public:

  struct mouse_data
  {
    float x;
    float y;
    bool leftButtonDown;
    bool rightButtonDown;
  };

  object_input_data();

  auto DisableMouse() -> void;
  [[nodiscard]] auto MouseEnabled() const -> bool;

  auto SetMouseData(const mouse_data& mouseData) -> void;
  auto GetMouseData() const -> const mouse_data&;

  auto SetPreviousMouseData(const mouse_data& mouseData) -> void;
  auto GetPreviousMouseData() const -> const mouse_data&;

  auto SetGamepadData(const gamepad_thumbstick& leftThumbstick) -> void;
  auto SetPreviousGamepadData(const gamepad_thumbstick& previousLeftThumbstick) -> void;

  auto SetCurrentButtonState(const gamepad_buttons& buttons) -> void;
  auto SetPreviousButtonState(const gamepad_buttons& buttons) -> void;

  [[nodiscard]] auto LeftMouseButtonClicked() const -> bool;
  [[nodiscard]] auto RightMouseButtonClicked() const -> bool;

  [[nodiscard]] auto GamepadAttached() const -> bool;
  [[nodiscard]] auto UpClicked() const -> bool;
  [[nodiscard]] auto DownClicked() const -> bool;

  [[nodiscard]] auto SelectPressed() const -> bool;
  [[nodiscard]] auto BackPressed() const -> bool;

private:

  bool m_mouseEnabled { true };
  mouse_data m_mouseData;
  mouse_data m_previousMouseData;
  bool m_gamepadAttached { false };
  gamepad_thumbstick m_leftThumbstick;
  gamepad_thumbstick m_previousLeftThumbstick;
  gamepad_buttons m_currentButtonState;
  gamepad_buttons m_previousButtonState;
};

inline auto object_input_data::DisableMouse() -> void
{
  m_mouseEnabled = false;
}

[[nodiscard]] inline auto object_input_data::MouseEnabled() const -> bool
{
  return m_mouseEnabled;
}

inline auto object_input_data::SetGamepadData(const gamepad_thumbstick& leftThumbstick) -> void
{
  m_gamepadAttached = true;
  m_leftThumbstick = leftThumbstick;
}

inline auto object_input_data::SetPreviousGamepadData(const gamepad_thumbstick& previousLeftThumbstick) -> void
{
  m_gamepadAttached = true;
  m_previousLeftThumbstick = previousLeftThumbstick;
}

inline auto object_input_data::SetCurrentButtonState(const gamepad_buttons& buttons) -> void
{
  m_currentButtonState = buttons;
}

inline auto object_input_data::SetPreviousButtonState(const gamepad_buttons& buttons) -> void
{
  m_previousButtonState = buttons;
}

[[nodiscard]] inline auto object_input_data::GamepadAttached() const -> bool
{
  return m_gamepadAttached;
}

[[nodiscard]] inline auto object_input_data::UpClicked() const -> bool
{
  return m_leftThumbstick.UpPressed() && !m_previousLeftThumbstick.UpPressed();
}

[[nodiscard]] inline auto object_input_data::DownClicked() const -> bool
{
  return m_leftThumbstick.DownPressed() && !m_previousLeftThumbstick.DownPressed();
}

[[nodiscard ]] inline auto object_input_data::SelectPressed() const -> bool
{
  return LeftMouseButtonClicked() ||
    ( m_currentButtonState.APressed() && !m_previousButtonState.APressed() ) ? true : false;
}

[[nodiscard ]] inline auto object_input_data::BackPressed() const -> bool
{
  return m_currentButtonState.BackPressed() && !m_previousButtonState.BackPressed() ? true : false;
}
