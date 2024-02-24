#pragma once

#include "level_objects.h"

class level_stage
{
public:

  [[nodiscard]] auto MineParameters() -> std::tuple<float, float, mine::type>
  {
    int mineTypeValue = m_dist(pseudo_random_generator::get());
    return { 20.0f, 200.0f, mineTypeValue > 5 ? mine::type::two : mine::type::one };
  }

  [[nodiscard]] auto MineCount() const -> int
  {
    return 10;
  }

private:

  std::uniform_int_distribution<int> m_dist { 1, 10 };

};
