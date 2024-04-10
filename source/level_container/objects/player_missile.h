#pragma once

#include "framework.h"
#include "base_object.h"

class player_missile : public base_object, object_velocity
{

public:

  player_missile(POINT_2F position, VELOCITY_2F velocity, int damage) : base_object { position, { 0.6f, 0.6f }, direct2d::CalculateDirection(velocity) }, object_velocity { velocity },
    m_damage { damage }
  {
  }

  [[nodiscard]] auto Damage()
  {
    return m_damage;
  }

  auto Update(float interval) noexcept -> void
  {
    base_object::Update(interval);
    m_position = UpdatePosition(m_position, interval);
    RotateBy(m_rotationSpeed * interval);
  }

  auto Update(float interval, POINT_2F targetPosition) noexcept -> void
  {
    base_object::Update(interval);
    MoveTowards(m_speed * interval, targetPosition);
    RotateBy(m_rotationSpeed * interval);
  }

private:

  inline static constexpr float m_speed { 200 };
  inline static constexpr float m_rotationSpeed { 400 };
  int m_damage;

};
