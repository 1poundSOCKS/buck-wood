#pragma once

#include "dynamic_object.h"
#include "level_objects.h"

using target_object = dynamic_object<level_target>;
using duct_fan_object = dynamic_object<duct_fan>;
using asteroid_object = dynamic_object<level_asteroid>;
using mine_object = dynamic_object<mine>;
using targetted_object_type = std::optional<targetted_object>;
