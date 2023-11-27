#pragma once

#include "keyboard_state.h"

class keyboard_reader
{

public:

  [[nodiscard]] auto CurrentState() const -> const keyboard_state&;
  [[nodiscard]] auto PreviousState() const -> const keyboard_state&;

  auto Update(IDirectInputDevice8* keyboard) -> void;

  [[nodiscard]] auto Pressed(int key) const -> bool;

private:

  keyboard_state m_currentState;
  keyboard_state m_previousState;
  
};

[[nodiscard]] inline auto keyboard_reader::CurrentState() const -> const keyboard_state&
{
  return m_currentState;
}

[[nodiscard]] inline auto keyboard_reader::PreviousState() const -> const keyboard_state&
{
  return m_previousState;
}

inline auto keyboard_reader::Update(IDirectInputDevice8* keyboard) -> void
{
  m_previousState = m_currentState;
  m_currentState.Update(keyboard);
}

[[nodiscard]] inline auto keyboard_reader::Pressed(int key) const -> bool
{
  return m_currentState.Down(key) && !m_previousState.Down(key);
}

class keyboard_reader2
{

public:

  static auto create(const winrt::com_ptr<IDirectInputDevice8>& keyboardDevice) -> void;
  static auto destroy() -> void;

  auto Update() -> void;
  [[nodiscard]] auto Pressed(int key) const -> bool;

private:

  keyboard_reader2(const winrt::com_ptr<IDirectInputDevice8>& keyboardDevice);

private:

  inline static keyboard_reader2* m_instance { nullptr };
  winrt::com_ptr<IDirectInputDevice8> m_keyboardDevice;
  std::unique_ptr<keyboard_state> m_currentState;
  std::unique_ptr<keyboard_state> m_previousState;

};
