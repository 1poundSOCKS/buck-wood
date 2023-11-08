#include "pch.h"
#include "common_menu_controller.h"
#include "effects_volume_slider.h"
#include "music_volume_slider.h"

[[nodiscard]] auto common_menu_controller::GetSettingsMenuDef(const render_target_area& area) -> menu_def
{
  menu_def menuDef { area.GetRect() };

  menuDef.AddButtonDef({ L"Effects volume", [this, area]() -> void
  {
    Open( { area.GetRect() } );
    GetCurrent().AddItem( effects_volume_slider {}, false );
    GetCurrent().AddItem( button { L"Back", [this](){ Close(); } }, true );
    GetCurrent().SelectFirstItem();
  }});
  
  menuDef.AddButtonDef({ L"Music volume", [this, area]() -> void
  {
    Open( { area.GetRect() } );
    GetCurrent().AddItem( music_volume_slider {}, false );
    GetCurrent().AddItem( button { L"Back", [this](){ Close(); } }, true );
    GetCurrent().SelectFirstItem();
  }});
  
  menuDef.AddButtonDef({ L"Back", [this]() -> void { Close(); }});
  
  return menuDef;
}
