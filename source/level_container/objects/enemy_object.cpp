#include "pch.h"
#include "enemy_object.h"

enemy_object::enemy_object(POINT_2F position, SCALE_2F scale, float angle) noexcept : 
  base_object{position, scale, angle}, m_maxHitpoints{1}, m_hitpoints{1}, m_cellPosition { cell_id { 0, 0 }, 0.5f }
{
}

auto enemy_object::SetCellId(cell_id cellId) noexcept -> void
{
  m_cellPosition.Set(cellId);
}

auto enemy_object::SetHitpoints(int value) noexcept -> void
{
  m_maxHitpoints = m_hitpoints = value;
}

auto enemy_object::ApplyDamage(int value) -> void
{
  m_hitpoints = std::max(0, m_hitpoints - value);
  m_destroyed = ( m_hitpoints == 0 );

  if( m_destroyed )
  {
    play_events::increment(play_events::counter_type::enemies_destroyed);
  }
}

[[nodiscard]] auto enemy_object::Health() const -> float
{
  return static_cast<float>(m_hitpoints) / static_cast<float>(m_maxHitpoints);
}

auto enemy_object::Update(float interval, object_cell_position::move_direction moveDirection, level_cell_collection& cells) noexcept -> void
{
  base_object::Update(interval);
}

auto enemy_object::Update(float interval, float moveDirection, level_cell_collection &cells) noexcept -> void
{
  auto cellMoveDirection = object_cell_position::MoveDirection(moveDirection);
  Update(interval, cellMoveDirection, cells);
}
