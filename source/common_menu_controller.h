#pragma once

#include "menu_controller.h"
#include "render_target_area.h"

class common_menu_controller : public menu_controller
{

public:

  [[nodiscard]] auto OpenSettingsMenu(const render_target_area& area) -> void;

};
