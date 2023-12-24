#pragma once

#include "particle.h"

class impact_particle : public particle
{

public:

  impact_particle() = default;
  impact_particle(const game_point& position);
  
  auto operator=(const impact_particle& particle) -> impact_particle& = default;
  
  [[nodiscard]] auto Lifespan() const -> float;
  [[nodiscard]] auto Position() const -> const game_point&;

private:

  inline static const float m_defaultLifespan { 0.3f };

};

inline impact_particle::impact_particle(const game_point& position) :
  particle { position, game_velocity {}, m_defaultLifespan }
{
}

[[nodiscard]] inline auto impact_particle::Lifespan() const -> float
{
  return particle::Lifespan();
}

[[nodiscard]] inline auto impact_particle::Position() const -> const game_point&
{
  return particle::Position();
}
