#include "pch.h"
#include "enemy_type_1.h"
#include "adjacent_floor_cells.h"

enemy_type_1::enemy_type_1(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : enemy_object { position, scale, angle }
{
}

auto enemy_type_1::Update(float interval, POINT_2F target, const level_cell_collection& cells) -> void
{
  base_object::Update(interval);

  m_destination = m_destination ? m_destination : NewDestination(target, cells);
  bool atDestination = m_destination ? MoveTowardsDestination(*m_destination, interval) : true;
  m_destination = atDestination ? std::nullopt : m_destination;
}

auto enemy_type_1::MoveTowardsDestination(level_cell_item destination, float interval) noexcept -> bool
{
  auto position = destination.Position();
  bool atDestination = MoveTowards(m_speed * interval, position);
  return atDestination;
}

auto enemy_type_1::NewDestination(POINT_2F target, const level_cell_collection& cells) -> std::optional<level_cell_item>
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
    if( distanceOfTargetToCell < distanceToTarget )
    {
      destinationCellId = cellId;
    }
  });

  return cells.Get(destinationCellId);
}

[[nodiscard]] auto enemy_type_1::CanShootAt(POINT_2F position) const -> bool
{
  return false;
}

[[nodiscard]] auto enemy_type_1::Reloaded() const -> bool
{
  return false;
}
