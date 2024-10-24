#include "pch.h"
#include "enemy_object.h"

enemy_object::enemy_object(POINT_2F position, SCALE_2F scale, float angle, int hitpoints) noexcept : 
  base_object{position, scale, angle}, m_maxHitpoints{hitpoints}, m_hitpoints{hitpoints}, m_remainingDamageInterval { 0.0f }
{
}

auto enemy_object::ApplyDamage(int value) -> void
{
  m_hitpoints = std::max(0, m_hitpoints - value);
  m_destroyed = ( m_hitpoints == 0 );
  m_remainingDamageInterval = m_startingDamageInterval;
}

[[nodiscard]] auto enemy_object::Health() const -> float
{
  return static_cast<float>(m_hitpoints) / static_cast<float>(m_maxHitpoints);
}

auto enemy_object::Update(float interval) noexcept -> void
{
  base_object::Update(interval);
  m_remainingDamageInterval = std::max(0.0f, m_remainingDamageInterval - interval);
}

auto enemy_object::RemainingDamageInterval() const noexcept -> float
{
  return m_remainingDamageInterval;
}
