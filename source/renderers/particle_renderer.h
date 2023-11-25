#pragma once

#include "render_types.h"
#include "particle.h"
#include "render_target.h"
#include "color_scale_brushes.h"

class particle_renderer
{

public:

  auto Write(const particle& particleInstance) const -> void;

private:

  color_scale_brushes m_brushes { color_scale { D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), 10 } };

};

inline auto particle_renderer::Write(const particle& particleInstance) const -> void
{
  static const auto rect = render_rect { -4, -4, 4, 4 };
  const auto& brush = m_brushes[particleInstance.Age() / particleInstance.Lifespan()];
  const auto& position = particleInstance.Position();
  const auto particleRect = render_rect { rect.left + position.x, rect.top + position.y, rect.right + position.x, rect.bottom + position.y };
  render_target::get()->FillRectangle(particleRect, brush.get());
}
