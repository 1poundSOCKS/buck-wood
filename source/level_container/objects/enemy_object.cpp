#include "pch.h"
#include "enemy_object.h"

enemy_object::enemy_object(cell_id position, level_cell_collection &cells) noexcept : enemy_object(cells.CellPosition(position), { 1.0f, 1.0f }, 0.0f)
{
}

enemy_object::enemy_object(POINT_2F position, SCALE_2F scale, float angle) noexcept : 
  base_object{position, scale, angle}, m_maxHitpoints{1}, m_hitpoints{1}, m_cellPosition { cell_id { 0, 0 }, 1.0f }
{
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

  cells.SetAsUnoccupied(m_cellPosition.Current());
  cells.SetAsUnoccupied(m_cellPosition.Next());

  auto cellId = m_cellPosition.Next();

  switch( moveDirection )
  {
    case object_cell_position::move_direction::up:
    {
      auto nextCellId = cellId.Get(cell_id::relative_position::above);
      moveDirection = cells.IsTypeOf(nextCellId, level_cell_type::floor) && cells.IsUnoccupied(nextCellId) ? object_cell_position::move_direction::up : object_cell_position::move_direction::none;
      break;      
    }
    case object_cell_position::move_direction::right:
    {
      auto nextCellId = cellId.Get(cell_id::relative_position::right);
      moveDirection = cells.IsTypeOf(nextCellId, level_cell_type::floor) && cells.IsUnoccupied(nextCellId) ? object_cell_position::move_direction::right : object_cell_position::move_direction::none;
      break;
    }
    case object_cell_position::move_direction::down:
    {
      auto nextCellId = cellId.Get(cell_id::relative_position::below);
      moveDirection = cells.IsTypeOf(nextCellId, level_cell_type::floor) && cells.IsUnoccupied(nextCellId) ? object_cell_position::move_direction::down : object_cell_position::move_direction::none;
      break;
    }
    case object_cell_position::move_direction::left:
    {
      auto nextCellId = cellId.Get(cell_id::relative_position::left);
      moveDirection = cells.IsTypeOf(nextCellId, level_cell_type::floor) && cells.IsUnoccupied(nextCellId) ? object_cell_position::move_direction::left : object_cell_position::move_direction::none;
      break;
    }
  }

  m_position = m_cellPosition(interval, moveDirection, cells.CellSize());  

  cells.SetAsOccupied(m_cellPosition.Current());
  cells.SetAsOccupied(m_cellPosition.Next());
}

auto enemy_object::PreErase(level_cell_collection& cells) const noexcept -> void
{
  cells.SetAsUnoccupied(m_cellPosition.Current());
  cells.SetAsUnoccupied(m_cellPosition.Next());
}
