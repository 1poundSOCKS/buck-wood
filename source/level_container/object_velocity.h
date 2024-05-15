#pragma once

#include "framework.h"

class object_velocity
{

public:

  object_velocity() = default;
  object_velocity(VELOCITY_2F value);

  [[nodiscard]] auto Get() const noexcept -> VELOCITY_2F;

  auto AdjustBy(VELOCITY_2F value) noexcept -> void;
  [[nodiscard]] auto UpdatePosition(POINT_2F position, float interval) const noexcept -> POINT_2F;

private:

  VELOCITY_2F m_value { 0, 0 };

};

inline object_velocity::object_velocity(VELOCITY_2F value) : m_value { value }
{
}

inline [[nodiscard]] auto object_velocity::Get() const noexcept -> VELOCITY_2F
{
  return m_value;
}

inline auto object_velocity::AdjustBy(VELOCITY_2F value) noexcept -> void
{
  m_value = { m_value.x + value.x, m_value.y + value.y };
}

inline [[nodiscard]] auto object_velocity::UpdatePosition(POINT_2F position, float interval) const noexcept -> POINT_2F
{
  return direct2d::CalculatePosition(position, m_value, interval);
}
