#include "pch.h"
#include "screen_input_state.h"

screen_input_state* screen_input_state::m_instance { nullptr };

auto screen_input_state::create(HINSTANCE instance, HWND window) -> void
{
  destroy();
  m_instance = new screen_input_state { instance, window };
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

auto screen_input_state::Update(const window_data& windowData, const render_target_mouse_data& mouseData) -> void
{
  m_previousWindowData = m_windowData;
  m_previousRenderTargetMouseData = m_renderTargetMouseData;
  m_windowData = windowData;
  m_renderTargetMouseData = mouseData;
  m_keyboardReader.Update(m_keyboard.get());
  m_gamepadReader.Update();
}

auto screen_input_state::CreateKeyboard(HINSTANCE instance, HWND window) -> winrt::com_ptr<IDirectInputDevice8>
{
  winrt::com_ptr<IDirectInput8> directInput;
  HRESULT hr = DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, directInput.put_void(), NULL);
  if( FAILED(hr) ) throw L"error";

  winrt::com_ptr<IDirectInputDevice8> keyboard;
  hr = directInput->CreateDevice(GUID_SysKeyboard, keyboard.put(), NULL);
  if( FAILED(hr) ) throw L"error";

  hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
  if( FAILED(hr) ) throw L"error";

  hr = keyboard->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
  if( FAILED(hr) ) throw L"error";

  keyboard->Acquire();

  return keyboard;
}
