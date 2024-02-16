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

  inline static const D2D1_RECT_F m_bulletRect { -5, -5, 5, 5 };

  color_scale_brushes m_modeOneBrushes { color_scale { screen_render_brush_type_one.Get(), screen_render_brush_black.Get(), 10 } };
  color_scale_brushes m_modeTwoBrushes { color_scale { screen_render_brush_type_two.Get(), screen_render_brush_black.Get(), 10 } };

};

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
