#include "pch.h"
#include "enemy_type_3.h"

// enemy_type_3::enemy_type_3(POINT_2F position, int hitpoints, float reloadTime) :
//   enemy_object { position, { 150, 150 }, 0, hitpoints }, m_reloadTimer { reloadTime }
enemy_type_3::enemy_type_3(POINT_2F position, SCALE_2F scale, float angle) :
  enemy_object { position, scale, angle }, m_reloadTimer { 0.5f }
{
}

auto enemy_type_3::Update(float interval, POINT_2F targetPosition) noexcept -> void
{
  base_object::Update(interval);
  m_angle = DirectionTo(targetPosition);
  m_reloaded = m_reloadTimer.Update(interval);
}

auto enemy_type_3::CanShootAt(POINT_2F position) const -> bool
{
  return m_reloaded;
}
