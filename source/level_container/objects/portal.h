#pragma once

#include "framework.h"
#include "base_object.h"

class portal : public base_object
{

public:

  enum class type { exit, entry };

  portal(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity);

  [[nodiscard]] auto Type() const -> type;

private:

  type m_type { type::exit };

};

inline portal::portal(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : base_object { position, scale, angle }
{
}

inline auto portal::Type() const -> type
{
  return m_type;
}
