#pragma once

#include "particle_type_renderer.h"

class particle_renderer
{

public:

  auto Write(const particle& particle) const -> void;

private:

  particle_type_renderer m_defaultParticleRenderer { 12, color_scale { screen_render_brush_particles.Get(), screen_render_brush_black.Get(), 10 } };
  particle_type_renderer m_impactParticleRenderer { 12, color_scale { screen_render_brush_impact_particles.Get(), screen_render_brush_black.Get(), 10 } };
  particle_type_renderer m_thrustParticleRenderer { 32, color_scale { screen_render_brush_thrust_particles.Get(), screen_render_brush_black.Get(), 10 } };

};

inline auto particle_renderer::Write(const particle& particle) const -> void
{
  switch( particle.Type() )
  {
    case particle::type::thrust:
      return m_thrustParticleRenderer.Write(particle);
    case particle::type::impact:
      return m_impactParticleRenderer.Write(particle);
    default:
      return m_defaultParticleRenderer.Write(particle);
  }
}
