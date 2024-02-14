#pragma once

#include "level_objects.h"

class level_stage
{
public:

  [[nodiscard]] auto MineParameters() -> std::tuple<float, float, mine::hardness_type>
  {
    int hardnessTypeValue = m_dist(pseudo_random_generator::get());
    return { 10.0f, 100.0f, hardnessTypeValue > 7 ? mine::hardness_type::tough : mine::hardness_type::soft };
  }

private:

  std::uniform_int_distribution<int> m_dist { 1, 10 };

};
