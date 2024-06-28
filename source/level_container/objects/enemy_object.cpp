#include "pch.h"
#include "enemy_object.h"

enemy_object::enemy_object(POINT_2F position, SCALE_2F scale, float angle) noexcept : 
  base_object { position, scale, angle }, m_maxHitpoints { 1 }, m_hitpoints { 1 }
{
}

auto enemy_object::SetHitpoints(int value) noexcept -> void
{
  m_hitpoints = value;
}

auto enemy_object::ApplyDamage(int value) -> void
{
  m_hitpoints = std::max(0, m_hitpoints - value);
  m_destroyed = m_hitpoints == 0;

  if( m_destroyed )
  {
    play_events::increment(play_events::counter_type::enemies_destroyed);
  }
}

[[nodiscard]] auto enemy_object::Health() const -> float
{
  return static_cast<float>(m_hitpoints) / static_cast<float>(m_maxHitpoints);
}
