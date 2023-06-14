#pragma once

#include "diagnostics.h"
#include "screen_transform.h"
#include "button_def.h"
#include "menu_def.h"
#include "mouse_cursor.h"

class main_menu_screen
{
public:

  main_menu_screen();
  auto Refresh(int64_t ticks) -> bool;

private:

  auto Update(int64_t frameInterval) -> void;
  auto Render() const -> void;

  [[nodiscard]] auto GetMenuDef() -> menu_def;

  mouse_cursor m_cursor;
  menu m_menu;
  
  bool m_continueRunning = true;
  bool m_startPlay = false;
};
