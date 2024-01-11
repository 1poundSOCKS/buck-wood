#pragma once

#include "particle_collision.h"

template <typename object_type, typename particle_type>
class particle_destruction_collision : public particle_collision<object_type, particle_type>
{

public:

  particle_destruction_collision() : particle_collision<object_type, particle_type>([](auto& object, auto& particle) { particle.Destroy(); }) {}

};
