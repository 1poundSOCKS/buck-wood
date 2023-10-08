#pragma once

#include "gamepad_reader.h"
#include "gamepad_buttons.h"

class gamepad_button_reader
{

public:

  gamepad_button_reader(const gamepad_reader& gamepadReader);

  [[nodiscard]] auto Pressed(int button) const -> bool;

private:

  gamepad_buttons m_currentButtons;
  gamepad_buttons m_previousButtons;

};

inline gamepad_button_reader::gamepad_button_reader(const gamepad_reader& gamepadReader) : 
  m_currentButtons { gamepadReader.CurrentState().Buttons() },
  m_previousButtons { gamepadReader.PreviousState().Buttons() }
{
}

[[nodiscard]] inline auto gamepad_button_reader::Pressed(int button) const -> bool
{
  return m_currentButtons.Down(button) && !m_previousButtons.Down(button);
}
