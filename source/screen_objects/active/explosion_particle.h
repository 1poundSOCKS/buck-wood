#pragma once

#include "particle.h"

class explosion_particle : public particle
{

public:
  explosion_particle() = default;
  explosion_particle(const game_point& position, const game_velocity& velocity, float lifespan);
  auto operator=(const explosion_particle& particle) -> explosion_particle& = default;
  [[nodiscard]] auto Lifespan() const -> float;
  [[nodiscard]] auto Position() const -> const game_point&;

};

inline explosion_particle::explosion_particle(const game_point& position, const game_velocity& velocity, float lifespan) :
  particle { position, velocity, lifespan }
{
}

[[nodiscard]] inline auto explosion_particle::Lifespan() const -> float
{
  return particle::Lifespan();
}

[[nodiscard]] inline auto explosion_particle::Position() const -> const game_point&
{
  return particle::Position();
}
