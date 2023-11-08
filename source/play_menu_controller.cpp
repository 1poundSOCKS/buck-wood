#include "pch.h"
#include "play_menu_controller.h"

auto play_menu_controller::OpenRoot(const render_target_area& area) -> void
{
  Open(area.GetRect());
  GetCurrent().AddItem( button { L"Resume", [this]() -> void { m_selection = selection::resume; } }, false);
  GetCurrent().AddItem( button { L"Settings", [this, area]() -> void { OpenSettingsMenu(area); } }, false);
  GetCurrent().AddItem( button { L"Quit", [this]() -> void { m_selection = selection::quit; } }, true);
  GetCurrent().SelectFirstItem();
}
