#pragma once

#include "main_menu_controller.h"

class main_menu_screen
{

public:

  main_menu_screen();
  auto Refresh(int64_t ticks) -> bool;

private:

  auto Update(int64_t frameInterval) -> bool;
  auto Render() const -> void;
  auto StartPlay() -> void;

private:

  main_menu_controller m_menuController;

};
