#include "pch.h"
#include "common_menu_controller.h"
#include "button.h"
#include "effects_volume_slider.h"
#include "music_volume_slider.h"

[[nodiscard]] auto common_menu_controller::OpenSettingsMenu() -> void
{
  Open();
  GetCurrent().AddItem<effects_volume_slider>();
  GetCurrent().AddItem<music_volume_slider>();
  GetCurrent().ResizeItems();
  GetCurrent().SelectFirstItem();
}
