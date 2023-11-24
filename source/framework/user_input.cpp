#include "pch.h"
#include "user_input.h"

user_input* user_input::m_instance { nullptr };

auto user_input::create(HINSTANCE instance, HWND wnd) -> void
{
  destroy();
  m_instance = new user_input { instance, wnd };
}

auto user_input::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

user_input::user_input(HINSTANCE instance, HWND window)
{
  m_keyboard = CreateKeyboard(instance, window);
}

auto user_input::Update() -> void
{
  m_keyboardReader.Update(m_keyboard.get());
  m_gamepadReader.Update();
}

auto user_input::CreateKeyboard(HINSTANCE instance, HWND window) -> winrt::com_ptr<IDirectInputDevice8>
{
  winrt::com_ptr<IDirectInput8> directInput;
  DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, directInput.put_void(), NULL);

  winrt::com_ptr<IDirectInputDevice8> keyboard;
  
  if( directInput )
  {
    directInput->CreateDevice(GUID_SysKeyboard, keyboard.put(), NULL);
  }

  if( keyboard )
  {
    keyboard->SetDataFormat(&c_dfDIKeyboard);
    keyboard->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    keyboard->Acquire();
  }

  return keyboard;
}
