#include "pch.h"
#include "menu_controller.h"

auto menu_controller::Open(const menu_def& def) -> void
{
  m_menus.emplace( def.CreateMenu() );
  m_menus.top().SelectFirstButton();
}

auto menu_controller::Close() -> void
{
  m_menus.pop();
}
