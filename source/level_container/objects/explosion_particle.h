#pragma once

#include "particle.h"

class explosion_particle : public particle
{

public:

  explosion_particle(D2D1_POINT_2F position, const game_velocity& velocity, float lifespan);
  
  auto operator=(const explosion_particle& particle) -> explosion_particle& = default;

};

inline explosion_particle::explosion_particle(D2D1_POINT_2F position, const game_velocity& velocity, float lifespan) :
  particle { position, velocity, lifespan }
{
}
