#pragma once

#include "diagnostics.h"
#include "screen_transform.h"
#include "button_def.h"
#include "menu_controller.h"
#include "mouse_cursor.h"

class main_menu_screen
{
public:

  main_menu_screen();
  auto Refresh(int64_t ticks) -> bool;

private:

  enum class menu_id { root, options };

  auto Update(int64_t frameInterval) -> void;
  auto Render() const -> void;

  [[nodiscard]] auto GetMenuDef(menu_id id) -> menu_def;

  menu_controller m_menuController;
  
  bool m_continueRunning = true;
  bool m_startPlay = false;
};
