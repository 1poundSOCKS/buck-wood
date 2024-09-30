#include "pch.h"
#include "object_velocity.h"

object_velocity::object_velocity() : m_value { 0.0f, 0.0f }
{
}

auto object_velocity::UpdateBy(VELOCITY_2F value, float interval) -> void
{
  m_value.x += value.x * interval;
  m_value.y += value.y * interval;
}

auto object_velocity::UpdatePosition(POINT_2F value, float interval) const noexcept -> POINT_2F
{
  return { value.x + ( m_value.x * interval ), value.y + ( m_value.y * interval )  };
}
