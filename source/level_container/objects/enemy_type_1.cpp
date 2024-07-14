#include "pch.h"
#include "enemy_type_1.h"
#include "adjacent_floor_cells.h"

enemy_type_1::enemy_type_1(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : enemy_object{position, scale, angle}
{
}

auto enemy_type_1::SetCellId(cell_id cellId) noexcept -> void
{
}

auto enemy_type_1::Update(float interval, POINT_2F target, level_cell_collection& cells) -> void
{
  base_object::Update(interval);

  cells.SetAsUnoccupied(cells.CellId(m_position));
  if( m_destination ) cells.SetAsUnoccupied(*m_destination);

  m_destination = m_destination ? m_destination : NewDestination(target, cells);
  bool atDestination = m_destination ? MoveTowardsDestination(*m_destination, interval, cells) : true;
  m_destination = atDestination ? std::nullopt : m_destination;

  cells.SetAsOccupied(cells.CellId(m_position));
  if( m_destination ) cells.SetAsOccupied(*m_destination);
}

auto enemy_type_1::MoveTowardsDestination(cell_id destination, float interval, level_cell_collection& cells) noexcept -> bool
{
  auto position = cells.CellPosition(destination);
  return MoveTowards(m_speed * interval, position);
}

auto enemy_type_1::NewDestination(POINT_2F target, const level_cell_collection& cells) -> std::optional<cell_id>
{
  auto distanceToTarget = direct2d::GetDistanceBetweenPoints(m_position, target);

  auto currentCellId = cells.CellId(m_position);
  auto destinationCellId = currentCellId;
  
  adjacent_floor_cells adjacentFloorCells { cells, currentCellId };

  adjacentFloorCells.Enumerate([&cells,&currentCellId,&destinationCellId,&target,distanceToTarget](const auto& cellId)
  {
    const auto& cell = cells.Get(cellId);
    auto positionOfCell = cell.Position();
    auto distanceOfTargetToCell = direct2d::GetDistanceBetweenPoints(target, positionOfCell);

    if( distanceOfTargetToCell < distanceToTarget && cells.IsUnoccupied(cellId) )
    {
      destinationCellId = cellId;
    }
  });

  return destinationCellId;
}

[[nodiscard]] auto enemy_type_1::CanShootAt(POINT_2F position) const -> bool
{
  return false;
}

[[nodiscard]] auto enemy_type_1::Reloaded() const -> bool
{
  return false;
}
