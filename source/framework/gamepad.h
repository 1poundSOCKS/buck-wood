#pragma once

#include <xinput.h>

class gamepad
{

public:

  static auto getState(DWORD userIndex) -> DWORD;

private:

  inline static gamepad* m_gamepad { nullptr };

  XINPUT_STATE m_state[4];

};

inline auto gamepad::getState(DWORD userIndex) -> DWORD
{
  if( userIndex < 0 || userIndex > 3 )
  {
    throw std::exception();
  }
  else
  {
    return XInputGetState(userIndex, &m_gamepad->m_state[userIndex]);
  }
}
