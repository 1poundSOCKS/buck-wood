#ifndef _main_window_
#define _main_window_

#include "window_data.h"
#include "render_target_mouse_data.h"

class main_window
{

public:

  static auto create(HINSTANCE instance, int cmdShow) -> void;
  static auto destroy() -> void;
  static auto handle() -> HWND;
  static auto data() -> const window_data&;

private:

  main_window(HINSTANCE instance, int cmdShow);
  static ATOM RegisterMainWindowClass(HINSTANCE instance);
  static HWND CreateMainWindow(HINSTANCE instance, int cmdShow, window_data& data);

private:

  static main_window* m_instance;
  HWND m_wnd { nullptr };
  window_data m_data;

};

inline auto main_window::create(HINSTANCE instance, int cmdShow) -> void
{
  destroy();
  m_instance = new main_window(instance, cmdShow);
}

inline auto main_window::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

inline auto main_window::handle() -> HWND
{
  return m_instance->m_wnd;
}

inline auto main_window::data() -> const window_data&
{
  return m_instance->m_data;
}

inline main_window::main_window(HINSTANCE instance, int cmdShow)
{
  RegisterMainWindowClass(instance);
  m_wnd = CreateMainWindow(instance, cmdShow, m_data);
}

#endif
