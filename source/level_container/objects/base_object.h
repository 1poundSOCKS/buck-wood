#pragma once

#include "framework.h"
#include "geometric_object.h"

class base_object : public geometric_object
{

public:

  base_object(POINT_2F position, SCALE_2F scale, float angle) noexcept : geometric_object { position, scale, angle }
  {
  }

  [[nodiscard]] auto Age() const noexcept -> float
  {
    return m_age;
  }

  [[nodiscard]] auto Destroyed() const noexcept -> bool
  {
    return m_destroyed;
  }

  auto Update(float interval) noexcept -> void
  {
    m_age += interval;
  }

  auto Destroy() noexcept -> void
  {
    m_destroyed = true;
  }

protected:

  float m_age { 0 };
  bool m_destroyed { false };

};
