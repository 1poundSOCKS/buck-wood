#pragma once

#include "framework.h"
#include "base_object.h"

class boundary_exit : public base_object
{

public:

  enum class type { left, top, right, bottom };

  boundary_exit(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) noexcept;
  auto SetType(type value) -> void;
  [[nodiscard]] auto Type() const noexcept -> type;

private:

  type m_type;

};

inline boundary_exit::boundary_exit(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) noexcept : 
  base_object(position, scale, angle), m_type { type::left }
{
}

inline auto boundary_exit::SetType(type value) -> void
{
  m_type = value;
}

inline [[nodiscard]] auto boundary_exit::Type() const noexcept -> type
{
  return m_type;
}
