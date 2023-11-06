#pragma once

#include "menu_def.h"
#include "render_target_area.h"

namespace options_menu_def
{
  [[nodiscard]] inline auto get(const render_target_area& area, menu_controller& menuController) -> menu_def
  {
    menu_def menuDef(area.GetRect());
    menuDef.AddButtonDef({ L"Effects volume", [area, &menuController]() -> void
    {
      menu_def menuDef { area.GetRect() };
      menuDef.AddButtonDef({ L"Back", [&menuController]() -> void { menuController.Close(); }});
      menuController.Open(menuDef);
    }});
    menuDef.AddButtonDef({ L"Music volume", [area, &menuController]() -> void
    {
      menu_def menuDef { area.GetRect() };
      menuDef.AddButtonDef({ L"Back", [&menuController]() -> void { menuController.Close(); }});
      menuController.Open(menuDef);
    }});
    menuDef.AddButtonDef({ L"Back", [&menuController]() -> void { menuController.Close(); }});
    return menuDef;
  };
};
