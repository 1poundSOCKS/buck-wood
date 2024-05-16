#pragma once

#include "framework.h"

class level_base
{

public:

  using cell_collection = std::set<std::tuple<int, int>>;

  virtual ~level_base() = default;

};
