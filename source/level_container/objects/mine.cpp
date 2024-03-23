#include "pch.h"
#include "mine.h"

mine::mine(type type, POINT_2F position, VELOCITY_2F velocity) : base_object { position, velocity, { 1.0f, 1.0f }, 0 },
  m_type { type }
{
}

auto mine::Update(float interval) -> void
{
  base_object::Update(interval);
  m_angle = direct2d::RotateAngle(m_angle, rotationSpeed * interval);
}
