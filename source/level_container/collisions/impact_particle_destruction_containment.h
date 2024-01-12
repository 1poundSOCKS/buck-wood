#pragma once

#include "particle_containment.h"

template <typename particle_type, typename impact_inserter_type>
class impact_particle_destruction_containment : public particle_containment<particle_type>
{

public:

  impact_particle_destruction_containment(impact_inserter_type impactInserter) : 
    particle_containment<particle_type>([this](auto& particle) { m_impactInserter = particle.Position(), particle.Destroy(); }),
    m_impactInserter { impactInserter }
  {
  }

private:

  impact_inserter_type m_impactInserter;

};
