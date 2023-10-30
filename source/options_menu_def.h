#pragma once

#include "menu_def.h"
#include "render_target_area.h"

[[nodiscard]] inline auto GetOptionsMenuDef(const render_target_area& area, 
  std::function<void()> effectsVolumeCallback, std::function<void()> musicVolumeCallback, std::function<void()> exitCallback) -> menu_def
{
  menu_def menuDef(area.GetRect());
  menuDef.AddButtonDef({ L"Effects volume", effectsVolumeCallback});
  menuDef.AddButtonDef({ L"Music volume", musicVolumeCallback});
  menuDef.AddButtonDef({ L"Back", exitCallback});
  return menuDef;
};