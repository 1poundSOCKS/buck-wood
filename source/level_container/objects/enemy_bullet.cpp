#include "pch.h"
#include "enemy_bullet.h"

enemy_bullet::enemy_bullet(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : 
  base_object { position, scale, angle }, m_velocity { velocity }
{
}

auto enemy_bullet::Update(float interval) -> void
{
  base_object::Update(interval);
  m_position.x += m_velocity.x * interval;
  m_position.y += m_velocity.y * interval;
  m_destroyed = Expired();
}
