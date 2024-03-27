#include "pch.h"
#include "enemy_type_2.h"

enemy_type_2::enemy_type_2(POINT_2F position, float reloadTime, int hitpoints) : base_object { position, { 1.5f, 1.5f }, 0 },
  m_reloadTimer { reloadTime }, m_maxHitpoints { hitpoints }, m_hitpoints { hitpoints }
{
}

auto enemy_type_2::Update(float interval) -> void
{
  base_object::Update(interval);
  m_position = m_randomMovement(m_position, interval);
  m_reloaded = m_reloadTimer.Update(interval);
}
