#include "pch.h"
#include "particle_collection.h"

auto particle_collection::Create(particle::type particleType, POINT_2F position, VELOCITY_2F velocity, float lifespan) noexcept -> particle&
{
  return m_particles.emplace_back(particle::type::impact, position, velocity, lifespan);
}

auto particle_collection::Update(float interval) noexcept -> void
{
  for( auto& particle : m_particles )
  {
    particle.Update(interval);
  }
}

auto particle_collection::EraseDestroyed() noexcept -> void
{
  std::erase_if(m_particles, [](const auto& particle) -> bool { return particle.Destroyed(); });
}
