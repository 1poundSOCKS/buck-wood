#pragma once

#include "main_window.h"
#include "keyboard_reader.h"
#include "gamepad_reader.h"

struct client_mouse_data
{
  RECT rect;
  float x = 0, y = 0;
};

class screen_input_state
{

public:

  static auto create(HINSTANCE instance, HWND window) -> void;
  static auto destroy() -> void;

  static auto update(const window_data& windowData, const render_target_mouse_data& mouseData) -> void;
  
  static auto windowData() -> const window_data&;
  static auto renderTargetMouseData() -> const render_target_mouse_data&;
  static auto previousRenderTargetMouseData() -> const render_target_mouse_data&;
  static auto keyboardReader() -> const keyboard_reader&;
  static auto gamepadReader() -> const gamepad_reader&;

private:

  screen_input_state(HINSTANCE instance, HWND window);
  auto Update(const window_data& windowData, const render_target_mouse_data& mouseData) -> void;
  static auto CreateKeyboard(HINSTANCE instance, HWND window) -> winrt::com_ptr<IDirectInputDevice8>;

private:

  static screen_input_state* m_instance;

  winrt::com_ptr<IDirectInputDevice8> m_keyboard;
  window_data m_windowData;
  window_data m_previousWindowData;
  keyboard_reader m_keyboardReader;
  render_target_mouse_data m_renderTargetMouseData;
  render_target_mouse_data m_previousRenderTargetMouseData;
  gamepad_reader m_gamepadReader;

};

winrt::com_ptr<IDirectInputDevice8> CreateKeyboard(HINSTANCE instance, HWND window);

inline auto screen_input_state::update(const window_data& windowData, const render_target_mouse_data& mouseData) -> void
{
  m_instance->Update(windowData, mouseData);
}

inline auto screen_input_state::keyboardReader() -> const keyboard_reader&
{
  return m_instance->m_keyboardReader;
}

inline auto screen_input_state::gamepadReader() -> const gamepad_reader&
{
  return m_instance->m_gamepadReader;
}

inline auto screen_input_state::windowData() -> const window_data&
{
  return m_instance->m_windowData;
}

inline auto screen_input_state::renderTargetMouseData() -> const render_target_mouse_data&
{
  return m_instance->m_renderTargetMouseData;
}

inline auto screen_input_state::previousRenderTargetMouseData() -> const render_target_mouse_data&
{
  return m_instance->m_previousRenderTargetMouseData;
}
