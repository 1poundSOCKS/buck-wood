#include "pch.h"
#include "play_menu_controller.h"

auto play_menu_controller::OpenRoot(const render_target_area& area) -> void
{
  Open(GetRootMenuDef(area));
}

[[nodiscard]] auto play_menu_controller::GetRootMenuDef(const render_target_area& area) -> menu_def
{
  menu_def menuDef(area.GetRect());

  menuDef.AddButtonDef({ L"Resume", [this]() -> void
  {
    m_selection = selection::resume;
  }});

  menuDef.AddButtonDef({ L"Settings", [this, area]() -> void
  {
    Open(GetSettingsMenuDef(area));
  }});

  menuDef.AddButtonDef({ L"Quit", [this]() -> void
  {
    m_selection = selection::quit;
  }});

  return menuDef;
}
