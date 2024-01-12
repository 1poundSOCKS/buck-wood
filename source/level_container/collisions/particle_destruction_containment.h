#pragma once

#include "particle_containment.h"

template <typename particle_type>
class particle_destruction_containment : public particle_containment<particle_type>
{

public:

  particle_destruction_containment() : 
    particle_containment<particle_type>([](auto& particle) { particle.Destroy(); })
  {
  }

};
