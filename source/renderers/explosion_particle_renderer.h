#pragma once

#include "render_types.h"
#include "explosion_particle.h"
#include "render_target.h"
#include "color_scale_brushes.h"

class explosion_particle_renderer
{

public:

  auto Write(const explosion_particle& particle) const -> void;

private:

  color_scale_brushes m_brushes { color_scale { D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), 10 } };

};

inline auto explosion_particle_renderer::Write(const explosion_particle& particle) const -> void
{
  static const auto rect = render_rect { -4, -4, 4, 4 };
  const auto& brush = m_brushes[particle.Age() / particle.Lifespan()];
  const auto& position = particle.Position();
  const auto particleRect = render_rect { rect.left + position.x, rect.top + position.y, rect.right + position.x, rect.bottom + position.y };
  render_target::renderTarget()->FillRectangle(particleRect, brush.get());
}
