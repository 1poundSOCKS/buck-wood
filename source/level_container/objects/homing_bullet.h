#pragma once

#include "bullet.h"
#include "targetted_object.h"

class homing_bullet : public bullet
{

public:

  homing_bullet(D2D1_POINT_2F position, direct2d::VELOCITY_2F velocity, std::optional<targetted_object> target) : bullet(position, velocity), m_target { target }
  {
  }

  auto Update(float interval) -> void
  {
    bullet::Update(interval, m_target && !m_target->Destroyed() ? std::optional<D2D1_POINT_2F>(m_target->Position()) : std::nullopt);
  }

  auto ValidateTargetPointer() -> void
  {
    m_target = m_target && m_target->Destroyed() ? std::nullopt : m_target;
  }

private:

  std::optional<targetted_object> m_target;

};
