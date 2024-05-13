#pragma once

#include "framework.h"
#include "base_object.h"
#include "object_velocity.h"

class enemy_bullet_1 : public base_object, public object_velocity
{

public:

  enum type { one, two };

  enemy_bullet_1(POINT_2F position, type type, VELOCITY_2F velocity);

  auto Update(float interval) -> void;

  [[nodiscard]] auto Type() const -> type;

private:

  static constexpr float m_spinRate { 400 };
  static constexpr float rotationSpeed { 300.0f };

  type m_type;

};

inline [[nodiscard]] auto enemy_bullet_1::Type() const -> type
{
  return m_type;
}
