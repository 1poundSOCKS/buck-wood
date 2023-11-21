#pragma once

#include "render_types.h"
#include "slider_control.h"
#include "player_shields.h"
#include "render_brushes.h"

class slider_control_renderer
{

public:

  auto Write(const player_shields& playerShields) const -> void;

private:

  filled_geometry_brushes m_brushes { D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), 3 };  

};

inline auto slider_control_renderer::Write(const player_shields& playerShields) const -> void
{
  auto shieldRemaining = 100.0f - playerShields.GetDamagePercentage();
  slider_control damageSlider = { render_rect { 50, 500, 100, 800 } };
  render_target::renderTarget()->FillRectangle(damageSlider.GetSliderRect(shieldRemaining), m_brushes.Fill().get());
  render_target::renderTarget()->DrawRectangle(damageSlider.GetBoundingRect(), m_brushes.Draw().get(), m_brushes.StrokeWidth());
}
