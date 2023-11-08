#include "pch.h"
#include "main_menu_controller.h"

auto main_menu_controller::OpenRoot(const render_target_area& area) -> void
{
  Open(area.GetRect());
  GetCurrent().AddItem( button { L"Start", [this]() -> void { m_selection = selection::start; }}, false);
  GetCurrent().AddItem( button { L"Settings", [this, area]() -> void { Open(GetSettingsMenuDef(area)); }}, false);
  GetCurrent().AddItem( button { L"Exit", [this]() -> void { m_selection = selection::exit; }}, true);
  GetCurrent().SelectFirstItem();
}
