#pragma once

#include "particle.h"

class explosion_particle : public particle
{

public:

  explosion_particle(D2D1_POINT_2F position, VELOCITY_2F velocity, float lifespan) : particle { position, velocity, lifespan }
  {
  }
  
  auto operator=(const explosion_particle& particle) -> explosion_particle& = default;

};
