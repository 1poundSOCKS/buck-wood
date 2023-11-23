#include "pch.h"
#include "screen_input_state.h"

screen_input_state* screen_input_state::m_instance { nullptr };

auto screen_input_state::create(HINSTANCE instance, HWND wnd) -> void
{
  destroy();
  m_instance = new screen_input_state { instance, wnd };
}

auto screen_input_state::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

screen_input_state::screen_input_state(HINSTANCE instance, HWND window)
{
  m_keyboard = CreateKeyboard(instance, window);
}

auto screen_input_state::Update(const window_data& windowData, const render_target_mouse_data& renderTargetMouseData) -> void
{
  m_previousWindowData = m_windowData;
  m_previousRenderTargetMouseData = m_renderTargetMouseData;
  m_windowData = windowData;
  m_renderTargetMouseData = renderTargetMouseData;
  m_keyboardReader.Update(m_keyboard.get());
  m_gamepadReader.Update();
}

auto screen_input_state::CreateKeyboard(HINSTANCE instance, HWND window) -> winrt::com_ptr<IDirectInputDevice8>
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
