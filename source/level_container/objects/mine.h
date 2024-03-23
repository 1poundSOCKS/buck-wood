#pragma once

#include "framework.h"
#include "base_object.h"

class mine : public base_object
{

public:

  enum type { one, two };

  mine(type type, POINT_2F position, VELOCITY_2F velocity);

  auto Update(float interval) -> void;

  [[nodiscard]] auto Type() const -> type;

private:

  static constexpr float m_spinRate { 400 };
  static constexpr float rotationSpeed { 200.0f };

  type m_type;

};

inline [[nodiscard]] auto mine::Type() const -> type
{
  return m_type;
}
