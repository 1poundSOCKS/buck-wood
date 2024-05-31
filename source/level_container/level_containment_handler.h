#pragma once

#include "default_object.h"

class level_containment_handler
{

public:

  auto operator()(default_object& object1, default_object& object2) -> void;

};
