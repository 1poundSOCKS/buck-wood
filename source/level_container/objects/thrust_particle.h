#pragma once

#include "particle.h"

class thrust_particle : public particle
{

public:

  thrust_particle(D2D1_POINT_2F position, VELOCITY_2F velocity, float lifespan) :
    particle { particle::type::thrust, position, velocity, lifespan }
  {
  }
  
  auto operator=(const thrust_particle& particle) -> thrust_particle& = default;

};
