#pragma once

#include "render_types.h"
#include "slider_control.h"

class slider_control_renderer
{

public:

  auto Write(const health_status& playerShields) const -> void;

private:

  winrt::com_ptr<ID2D1SolidColorBrush> m_fillBrush { screen_render_brush_grey.CreateBrush() };
  winrt::com_ptr<ID2D1SolidColorBrush> m_drawBrush { screen_render_brush_white.CreateBrush() };

};

inline auto slider_control_renderer::Write(const health_status& playerShields) const -> void
{
  auto shieldRemaining = 100.0f - playerShields.GetDamagePercentage();
  slider_control damageSlider = { render_rect { 50, 500, 100, 800 } };
  render_target::get()->FillRectangle(damageSlider.GetSliderRect(shieldRemaining), m_fillBrush.get());
  render_target::get()->DrawRectangle(damageSlider.GetBoundingRect(), m_drawBrush.get(), 5);
}
