#include "pch.h"
#include "enemy_object.h"

enemy_object::enemy_object(POINT_2F position, SCALE_2F scale, float angle) noexcept : 
  base_object { position, scale, angle }, m_maxHitpoints { 1 }, m_hitpoints { 1 }, m_cells { std::make_shared<level_cell_collection>(1, 1) }
{
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

auto enemy_object::SetCells(std::shared_ptr<level_cell_collection> cells) -> void
{
  m_cells = cells;
}

[[nodiscard]] auto enemy_object::Health() const -> float
{
  return static_cast<float>(m_hitpoints) / static_cast<float>(m_maxHitpoints);
}
