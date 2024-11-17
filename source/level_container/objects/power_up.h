#pragma once

#include "framework.h"
#include "base_object.h"

class power_up : public base_object
{

public:

  enum class type { time_bonus, level_completion };

  power_up(POINT_2F position, SCALE_2F scale, float angle, type objectType) : base_object { position , scale, angle }, m_type { objectType }
  {
  }

  [[nodiscard]] auto Type() const noexcept -> type
  {
    return m_type;
  }

  auto Update(float interval) noexcept -> void
  {
    base_object::Update(interval);
  }

private:

  type m_type;

};
