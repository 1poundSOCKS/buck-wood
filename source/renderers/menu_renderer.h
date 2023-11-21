#pragma once

#include "menu_item.h"
#include "screen_render_brush_defs.h"
#include "render_text_format_def.h"
#include "render_types.h"

class menu_renderer
{

public:

  auto Write(const menu_item& menuItem) const -> void;
  auto Write(const button& buttonObject) const -> void;
  auto Write(const setting_slider& settingSlider) const -> void;

private:

  render_brush m_buttonBrush { screen_render_brush_white.CreateBrush() };
  render_brush m_buttonBorderBrush { screen_render_brush_grey.CreateBrush() };
  render_brush m_buttonHoverBrush { screen_render_brush_green.CreateBrush() };
  render_brush m_textHoverBrush { screen_render_brush_green.CreateBrush() };
  render_brush m_textBrush { screen_render_brush_grey.CreateBrush() };

  render_text_format m_defaultText { render_text_format_def { L"System Bold", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 100 }.CreateTextFormat() };
  render_text_format m_hoverText { render_text_format_def { L"System Bold", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 100 }.CreateTextFormat() };
  render_text_format m_smallText { render_text_format_def { L"System Bold", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 50 }.CreateTextFormat() };

};
