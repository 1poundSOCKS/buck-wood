#pragma once

#include "dynamic_object.h"
#include "level_objects.h"

using target_object = dynamic_object<level_target>;
using duct_fan_object = dynamic_object<duct_fan>;
using asteroid_object = dynamic_object<level_asteroid>;
using mine_object = dynamic_object<mine>;

using target_collection = std::vector<target_object>;
using duct_fan_collection = std::vector<duct_fan_object>;
using asteroid_collection = std::vector<asteroid_object>;
using mine_collection = std::list<mine_object>;

using bullet_collection = std::list<homing_bullet>;
using explosion_particle_collection  = std::list<explosion_particle>;
using impact_particle_collection  = std::list<impact_particle>;
using thrust_particle_collection = std::list<thrust_particle>;

using targetted_object_type = std::optional<targetted_object>;

using explosion_collection = std::vector<D2D1_POINT_2F>;
using impact_collection = std::vector<D2D1_POINT_2F>;
