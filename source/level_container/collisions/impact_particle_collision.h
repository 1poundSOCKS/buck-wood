#pragma once

#include "particle_collision.h"

template <typename geometry_type, typename particle_type, typename impact_inserter_type>
class impact_particle_collision : public particle_collision<geometry_type, particle_type>
{

public:

  impact_particle_collision(impact_inserter_type impactInserter) : 
    particle_collision<geometry_type, particle_type>([this](auto& geometry, auto& particle) { m_impactInserter = particle.Position(), particle.Destroy(); }),
    m_impactInserter { impactInserter }
  {
  }

private:

  impact_inserter_type m_impactInserter;

};
