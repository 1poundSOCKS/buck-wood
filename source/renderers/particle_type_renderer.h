#pragma once

#include "framework.h"
#include "objects/particle.h"
#include "color_scale_brushes.h"

class particle_type_renderer
{

public:

  template <class... Args> particle_type_renderer(Args&&... args);
  auto Write(const particle& particleInstance) const -> void;

private:

  constexpr static float m_renderRectSize { 8 };
  constexpr static D2D1_RECT_F m_renderRect { -m_renderRectSize, -m_renderRectSize, m_renderRectSize, m_renderRectSize };
  color_scale_brushes m_brushes;

};

template <class... Args> particle_type_renderer::particle_type_renderer(Args&&... args) : 
  m_brushes(std::forward<Args>(args)...)
{
}

inline auto particle_type_renderer::Write(const particle& particleInstance) const -> void
{
  const auto& brush = m_brushes[particleInstance.Age() / particleInstance.Lifespan()];
  auto position = particleInstance.Position();
  const auto particleRect = D2D1_RECT_F { m_renderRect.left + position.x, m_renderRect.top + position.y, m_renderRect.right + position.x, m_renderRect.bottom + position.y };
  render_target::get()->FillRectangle(particleRect, brush.get());
}
