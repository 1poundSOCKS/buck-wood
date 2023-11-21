#pragma once

#include "menu_item.h"
#include "render_text.h"
#include "screen_render_brush_defs.h"

class menu_renderer
{

public:

  auto Write(const menu_item& menuItem) const -> void;
  auto Write(const button& buttonObject) const -> void;
  auto Write(const setting_slider& settingSlider) const -> void;

private:

  winrt::com_ptr<ID2D1SolidColorBrush> m_buttonBrush { screen_render_brush_white.CreateBrush() };
  winrt::com_ptr<ID2D1SolidColorBrush> m_buttonBorderBrush { screen_render_brush_grey.CreateBrush() };
  winrt::com_ptr<ID2D1SolidColorBrush> m_buttonHoverBrush { screen_render_brush_green.CreateBrush() };
  winrt::com_ptr<ID2D1SolidColorBrush> m_textHoverBrush { screen_render_brush_green.CreateBrush() };
  winrt::com_ptr<ID2D1SolidColorBrush> m_textBrush { screen_render_brush_grey.CreateBrush() };
  render_text m_renderText;

};
