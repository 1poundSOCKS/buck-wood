#pragma once

#include "screen_input_state.h"
#include "gamepad_thumbstick.h"
#include "gamepad_button_reader.h"

class menu_control_data
{

public:

  menu_control_data(const screen_input_state& screenInputState);

  [[nodiscard]] auto GamepadAttached() const -> bool;

  [[nodiscard]] auto Up() const -> bool;
  [[nodiscard]] auto Down() const -> bool;
  [[nodiscard]] auto Left() const -> bool;
  [[nodiscard]] auto Right() const -> bool;
  [[nodiscard]] auto Select() const -> bool;
  [[nodiscard]] auto Back() const -> bool;

private:

  bool m_gamepadAttached { false };
  bool m_up { false };
  bool m_down { false };
  bool m_left { false };
  bool m_right { false };
  bool m_select { false };
  bool m_back { false };

};

[[nodiscard]] inline auto menu_control_data::GamepadAttached() const -> bool
{
  return m_gamepadAttached;
}

[[nodiscard]] inline auto menu_control_data::Up() const -> bool
{
  return m_up;
}

[[nodiscard]] inline auto menu_control_data::Down() const -> bool
{
  return m_down;
}

[[nodiscard]] inline auto menu_control_data::Left() const -> bool
{
  return m_left;
}

[[nodiscard]] inline auto menu_control_data::Right() const -> bool
{
  return m_right;
}

[[nodiscard ]] inline auto menu_control_data::Select() const -> bool
{
  return m_select;
}

[[nodiscard ]] inline auto menu_control_data::Back() const -> bool
{
  return m_back;
}
