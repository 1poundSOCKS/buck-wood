#pragma once

#include "particle.h"

class impact_particle : public particle
{

public:

  impact_particle(D2D1_POINT_2F position);
  
  auto operator=(const impact_particle& particle) -> impact_particle& = default;
  
  [[nodiscard]] auto Lifespan() const -> float;
  [[nodiscard]] auto Position() const -> D2D1_POINT_2F;

private:

  inline static const float m_defaultLifespan { 0.3f };

};

inline impact_particle::impact_particle(D2D1_POINT_2F position) :
  particle { position, { 0, 0 }, m_defaultLifespan }
{
}

[[nodiscard]] inline auto impact_particle::Lifespan() const -> float
{
  return particle::Lifespan();
}

[[nodiscard]] inline auto impact_particle::Position() const -> D2D1_POINT_2F
{
  return particle::Position();
}
