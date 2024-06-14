#pragma once

#include "framework.h"
#include "objects/particle.h"

class particle_collection
{

public:

  using value_type = std::list<particle>;
  using iterator = value_type::iterator;
  using const_iterator = value_type::const_iterator;

public:

  auto Create(particle::type particleType, POINT_2F position, VELOCITY_2F velocity, float lifespan) noexcept -> particle&;
  auto Add(std::ranges::input_range auto&& particles) noexcept -> void;
  auto Update(float interval) noexcept -> void;
  auto EraseDestroyed() noexcept -> void;
  auto Visit(auto&& visitor) const noexcept -> void;

  [[nodiscard]] auto begin() noexcept -> iterator;
  [[nodiscard]] auto begin() const noexcept -> const_iterator;
  [[nodiscard]] auto end() noexcept -> iterator;
  [[nodiscard]] auto end() const noexcept -> const_iterator;

private:

  value_type m_particles;

};

auto particle_collection::Add(std::ranges::input_range auto &&particles) noexcept -> void
{
  std::ranges::copy(particles, std::back_inserter(m_particles));
}

auto particle_collection::Visit(auto &&visitor) const noexcept -> void
{
  for( const auto& particle : m_particles )
  {
    visitor(particle);
  }
}

inline auto particle_collection::begin() noexcept -> iterator
{
  return std::begin(m_particles);
}

inline auto particle_collection::begin() const noexcept -> const_iterator
{
  return std::begin(m_particles);
}

inline auto particle_collection::end() noexcept -> iterator
{
  return std::end(m_particles);
}

inline auto particle_collection::end() const noexcept -> const_iterator
{
  return std::end(m_particles);
}
