#pragma once

#include "particle.h"

class thrust_particle : public particle
{

public:

  thrust_particle() = default;
  thrust_particle(const game_point& position, const game_velocity& velocity, float lifespan);
  
  auto operator=(const thrust_particle& particle) -> thrust_particle& = default;

};

inline thrust_particle::thrust_particle(const game_point& position, const game_velocity& velocity, float lifespan) :
  particle { position, velocity, lifespan }
{
}
