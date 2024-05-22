#include "pch.h"
#include "enemy_type_3.h"

enemy_type_3::enemy_type_3(POINT_2F position, int hitpoints, float reloadTime) :
  enemy_object { position, { 1.5, 1.5 }, 0, hitpoints }, m_reloadTimer { reloadTime }
{
}

auto enemy_type_3::Update(float interval) noexcept -> void
{
  base_object::Update(interval);
  m_reloaded = m_reloadTimer.Update(interval);
}

auto enemy_type_3::CanShootAt(POINT_2F position) const -> bool
{
  return m_reloaded;
}
