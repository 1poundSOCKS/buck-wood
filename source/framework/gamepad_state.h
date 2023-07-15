#pragma once

#include <xinput.h>

class gamepad_state
{

public:

  gamepad_state() = default;
  gamepad_state(DWORD userIndex);

  auto Update() -> DWORD;

  [[nodiscard]] auto Connected() -> bool;

private:

  DWORD m_userIndex { 0 };
  bool m_connected { false };
  XINPUT_STATE m_state;

};

inline gamepad_state::gamepad_state(DWORD userIndex) : m_userIndex { userIndex }
{
  if( m_userIndex < 0 || m_userIndex > 3 )
  {
    throw std::exception();
  }
}

inline auto gamepad_state::Update() -> DWORD
{
  auto stateReturn = XInputGetState(m_userIndex, &m_state);
  m_connected = stateReturn == ERROR_SUCCESS ? true : false;
  return stateReturn;
}

[[nodiscard]] inline auto gamepad_state::Connected() -> bool
{
  return m_connected;
}
