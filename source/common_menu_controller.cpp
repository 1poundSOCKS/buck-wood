#include "pch.h"
#include "common_menu_controller.h"
#include "button.h"
#include "effects_volume_slider.h"
#include "music_volume_slider.h"

[[nodiscard]] auto common_menu_controller::OpenSettingsMenu(const render_target_area& area) -> void
{
  Open(area.GetRect());
  GetCurrent().AddItem( button { L"Effects volume", [this, area]() -> void { OpenEffectsVolumeMenu(area); } }, false);
  GetCurrent().AddItem( button { L"Music volume", [this, area]() -> void { OpenMusicVolumeMenu(area); } }, true);
  GetCurrent().SelectFirstItem();
}

[[nodiscard]] auto common_menu_controller::OpenEffectsVolumeMenu(const render_target_area& area) -> void
{
  Open(area.GetRect());
  GetCurrent().AddItem( effects_volume_slider {}, true );
  GetCurrent().SelectFirstItem();
}

[[nodiscard]] auto common_menu_controller::OpenMusicVolumeMenu(const render_target_area& area) -> void
{
  Open(area.GetRect());
  GetCurrent().AddItem( music_volume_slider {}, true );
  GetCurrent().SelectFirstItem();
}
