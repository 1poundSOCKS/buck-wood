#include "pch.h"
#include "enemy_bullet_1.h"

// enemy_bullet_1::enemy_bullet_1(POINT_2F position, type type, VELOCITY_2F velocity) : base_object { position, { 1.5f, 1.5f }, 0 }, m_velocity { velocity }, m_type { type }
enemy_bullet_1::enemy_bullet_1(POINT_2F position, SCALE_2F scale, float angle) : 
  base_object { position, scale, angle }, m_velocity { 0, 0 }
{
}

auto enemy_bullet_1::Update(float interval) -> void
{
  base_object::Update(interval);
  m_position = m_velocity.UpdatePosition(m_position, interval);
  m_angle = direct2d::RotateAngle(m_angle, rotationSpeed * interval);
}
