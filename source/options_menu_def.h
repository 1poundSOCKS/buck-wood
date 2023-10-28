#pragma once

#include "menu_def.h"
#include "render_target_area.h"

[[nodiscard]] inline auto GetOptionsMenuDef(const render_target_area& area, std::function<void()> callback) -> menu_def
{
  menu_def menuDef(area.GetRect());

  menuDef.AddButtonDef({ L"Effects volume", []()
  {
  }});

  menuDef.AddButtonDef({ L"Music volume", []()
  {
  }});

  menuDef.AddButtonDef({ L"Back", callback});

  menuDef.UpdateButtons();

  return menuDef;
};
