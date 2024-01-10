#pragma once

#include "particle.h"

class thrust_particle : public particle
{

public:

  thrust_particle() = default;
  thrust_particle(D2D1_POINT_2F position, const game_velocity& velocity, float lifespan);
  
  auto operator=(const thrust_particle& particle) -> thrust_particle& = default;

};

inline thrust_particle::thrust_particle(D2D1_POINT_2F position, const game_velocity& velocity, float lifespan) :
  particle { position, velocity, lifespan }
{
}
