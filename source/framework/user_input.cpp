#include "pch.h"
#include "user_input.h"

user_input* user_input::m_instance { nullptr };

auto user_input::create(const winrt::com_ptr<IDirectInputDevice8>& keyboardDevice) -> void
{
  destroy();
  m_instance = new user_input { keyboardDevice };
}

auto user_input::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

user_input::user_input(const winrt::com_ptr<IDirectInputDevice8>& keyboardDevice) : m_keyboard { keyboardDevice }
{
}

auto user_input::Update() -> void
{
  m_keyboardReader.Update(m_keyboard.get());
  m_gamepadReader.Update();
}
