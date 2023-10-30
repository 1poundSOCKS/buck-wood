#pragma once

#include "gamepad_state.h"

class gamepad_reader
{

public:

  [[nodiscard]] auto CurrentState() const -> const gamepad_state&;
  [[nodiscard]] auto PreviousState() const -> const gamepad_state&;

  auto Update() -> DWORD;

  [[nodiscard]] auto Pressed(int button) const -> bool;

private:

  gamepad_state m_currentState;
  gamepad_state m_previousState;

};

[[nodiscard]] inline auto gamepad_reader::CurrentState() const -> const gamepad_state&
{
  return m_currentState;
}

[[nodiscard]] inline auto gamepad_reader::PreviousState() const -> const gamepad_state&
{
  return m_previousState;
}

inline auto gamepad_reader::Update() -> DWORD
{
  m_previousState = m_currentState;
  return m_currentState.Update();
}

[[nodiscard]] inline auto gamepad_reader::Pressed(int button) const -> bool
{
  return m_currentState.Down(button) && !m_previousState.Down(button);
}
