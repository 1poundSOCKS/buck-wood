#include "pch.h"
#include "main_menu_controller.h"

auto main_menu_controller::OpenRoot(const render_target_area& area) -> void
{
  Open(GetRootMenuDef(area));
}

[[nodiscard]] auto main_menu_controller::GetRootMenuDef(const render_target_area& area) -> menu_def
{
  menu_def menuDef(area.GetRect());

  menuDef.AddButtonDef({ L"Start", [this]() -> void
  {
    m_selection = selection::start;
  }});

  menuDef.AddButtonDef({ L"Settings", [this, area]() -> void
  {
    Open(GetSettingsMenuDef(area));
  }});

  menuDef.AddButtonDef({ L"Exit", [this]() -> void
  {
    m_selection = selection::exit;
  }});

  return menuDef;
}
