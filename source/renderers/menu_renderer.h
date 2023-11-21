#pragma once

#include "menu_item.h"
#include "render_brushes.h"
#include "render_text.h"

class menu_renderer
{

public:

  auto Write(const menu_item& menuItem) const -> void;
  auto Write(const button& buttonObject) const -> void;
  auto Write(const setting_slider& settingSlider) const -> void;

private:

  menu_brushes m_menuBrushes;
  render_text m_renderText;

};
