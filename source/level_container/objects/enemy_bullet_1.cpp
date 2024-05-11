#include "pch.h"
#include "enemy_bullet_1.h"

enemy_bullet_1::enemy_bullet_1(type type, POINT_2F position, VELOCITY_2F velocity) : base_object { position, { 1.5f, 1.5f }, 0 }, object_velocity { velocity }, m_type { type }
{
}

auto enemy_bullet_1::Update(float interval) -> void
{
  base_object::Update(interval);
  m_position = object_velocity::UpdatePosition(m_position, interval);
  m_angle = direct2d::RotateAngle(m_angle, rotationSpeed * interval);
}
