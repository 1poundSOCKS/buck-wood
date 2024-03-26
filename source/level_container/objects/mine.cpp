#include "pch.h"
#include "mine.h"

mine::mine(type type, POINT_2F position, VELOCITY_2F velocity) : base_object { position, { 1.0f, 1.0f }, 0 }, object_velocity { velocity }, m_type { type }
{
}

auto mine::Update(float interval) -> void
{
  base_object::Update(interval);
  m_position = object_velocity::UpdatePosition(m_position, interval);
  m_angle = direct2d::RotateAngle(m_angle, rotationSpeed * interval);
}
