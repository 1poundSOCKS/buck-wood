#include "pch.h"
#include "keyboard_reader.h"

auto keyboard_reader2::create(const winrt::com_ptr<IDirectInputDevice8>& keyboardDevice) -> void
{
  destroy();
  m_instance = new keyboard_reader2 { keyboardDevice };
}

auto keyboard_reader2::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

keyboard_reader2::keyboard_reader2(const winrt::com_ptr<IDirectInputDevice8>& keyboardDevice) : m_keyboardDevice { keyboardDevice }
{
}
