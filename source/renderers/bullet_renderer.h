#pragma once

#include "objects/bullet.h"
#include "color_scale_brushes.h"

class bullet_renderer
{

public:

  auto Write(const bullet& bulletInstance) const -> void;

private:

  static const D2D1_RECT_F m_bulletRect;

  color_scale_brushes m_brushes { color_scale { D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), 10 } };
  
};

inline const D2D1_RECT_F bullet_renderer::m_bulletRect { -5, -5, 5, 5 };

inline auto bullet_renderer::Write(const bullet& bulletInstance) const -> void
{
  const auto& position = bulletInstance.Position();
  const auto& brush = m_brushes[bulletInstance.DistanceTravelled() / bulletInstance.Range()];
  D2D1_RECT_F renderRect { m_bulletRect.left + position.x, m_bulletRect.top + position.y, m_bulletRect.right + position.x, m_bulletRect.bottom + position.y };
  render_target::get()->FillRectangle(renderRect, brush.get());
}
