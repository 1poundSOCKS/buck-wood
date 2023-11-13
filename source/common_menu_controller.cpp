#include "pch.h"
#include "common_menu_controller.h"
#include "button.h"
#include "effects_volume_slider.h"
#include "music_volume_slider.h"

[[nodiscard]] auto common_menu_controller::OpenSettingsMenu(const render_target_area& area) -> void
{
  Open(area.GetRect());
  GetCurrent().AddItem( effects_volume_slider {}, false );
  GetCurrent().AddItem( music_volume_slider {}, true );
  GetCurrent().SelectFirstItem();
}
