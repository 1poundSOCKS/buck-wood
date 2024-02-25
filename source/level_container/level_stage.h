#pragma once

#include "level_objects.h"

class level_stage
{
public:

  [[nodiscard]] auto MineType() -> mine::type
  {
    int mineTypeValue = m_dist(pseudo_random_generator::get());
    return mineTypeValue > 5 ? mine::type::two : mine::type::one;
  }

private:

  std::uniform_int_distribution<int> m_dist { 1, 10 };

};
