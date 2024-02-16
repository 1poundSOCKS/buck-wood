#pragma once

#include "objects/bullet.h"
#include "color_scale_brushes.h"

class bullet_renderer
{

public:

  auto Write(const bullet& bulletInstance) const -> void;

private:

  [[nodiscard]] auto GetBulletBrush(const bullet& bullet) const -> const render_brush&;

private:

  static const D2D1_RECT_F m_bulletRect;
  color_scale_brushes m_modeOneBrushes { color_scale { D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f), D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), 10 } };
  color_scale_brushes m_modeTwoBrushes { color_scale { D2D1::ColorF(0.0f, 0.0f, 1.0f, 1.0f), D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), 10 } };
  render_brush m_modeOneBrush { screen_render_brush_red.CreateBrush() };
  render_brush m_modeTwoBrush { screen_render_brush_blue.CreateBrush() };

};

inline const D2D1_RECT_F bullet_renderer::m_bulletRect { -5, -5, 5, 5 };

inline auto bullet_renderer::Write(const bullet& bullet) const -> void
{
  const auto& position = bullet.Position();
  const auto& brush = GetBulletBrush(bullet);
  D2D1_RECT_F renderRect { m_bulletRect.left + position.x, m_bulletRect.top + position.y, m_bulletRect.right + position.x, m_bulletRect.bottom + position.y };
  render_target::get()->FillRectangle(renderRect, brush.get());
}

inline [[nodiscard]] auto bullet_renderer::GetBulletBrush(const bullet& bullet) const -> const render_brush&
{
  auto distanceRatio = bullet.DistanceTravelled() / bullet.Range();

  switch( bullet.DamageMode() )
  {
    case bullet::damage_mode::two:
      return m_modeTwoBrushes[distanceRatio];
    default:
      return m_modeOneBrushes[distanceRatio];
  }
}
