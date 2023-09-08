#pragma once

#include "main_window.h"
#include "gamepad_thumbstick.h"

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

  auto SetMouseData(const mouse_data& mouseData) -> void;
  auto GetMouseData() const -> const mouse_data&;

  auto SetPreviousMouseData(const mouse_data& mouseData) -> void;
  auto GetPreviousMouseData() const -> const mouse_data&;

  auto SetGamepadData(const gamepad_thumbstick& leftThumbstick) -> void;
  auto SetPreviousGamepadData(const gamepad_thumbstick& previousLeftThumbstick) -> void;

  [[nodiscard]] auto LeftMouseButtonClicked() const -> bool;
  [[nodiscard]] auto RightMouseButtonClicked() const -> bool;

  [[nodiscard]] auto GamepadAttached() const -> bool;
  [[nodiscard]] auto UpClicked() const -> bool;
  [[nodiscard]] auto DownClicked() const -> bool;

private:

  mouse_data m_mouseData;
  mouse_data m_previousMouseData;
  bool m_gamepadAttached { false };
  gamepad_thumbstick m_leftThumbstick;
  gamepad_thumbstick m_previousLeftThumbstick;
};

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
