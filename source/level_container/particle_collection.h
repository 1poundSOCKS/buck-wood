#pragma once

template <typename type>
class particle_collection
{

public:

  particle_collection() = default;

  template <typename...Args> auto Create(Args...args) -> void
  {
    m_particles.emplace_back(std::forward<Args>(args)...);
  }

  auto Create(std::ranges::input_range auto&& particles)
  {
    std::ranges::copy(particles, std::back_inserter(m_particles));
  }

  auto begin() const
  {
    return std::begin(m_particles);
  }

  auto end() const
  {
    return std::end(m_particles);
  }

  auto begin()
  {
    return std::begin(m_particles);
  }

  auto end()
  {
    return std::end(m_particles);
  }

  auto Update(float interval) -> void
  {
    for( auto& particle : m_particles )
    {
      particle.Update(interval);
    }
  }

  auto EraseDestroyed() -> void
  {
    auto particle = std::begin(m_particles);

    while( particle != std::end(m_particles) )
    {
      particle = particle->Destroyed() ? m_particles.erase(particle) : ++particle;
    }
  }

private:

  using collection_type = std::list<type>;

  collection_type m_particles;

};
