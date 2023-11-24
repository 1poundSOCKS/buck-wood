#pragma once

#include "main_menu_controller.h"

class main_menu_screen
{
public:

  main_menu_screen();
  auto Refresh(int64_t ticks) -> bool;

private:

  enum class menu_id { root, options };

  auto Update(int64_t frameInterval) -> void;
  auto Render() const -> void;

private:

  main_menu_controller m_menuController;
  bool m_continueRunning = true;
  bool m_startPlay = false;
};
