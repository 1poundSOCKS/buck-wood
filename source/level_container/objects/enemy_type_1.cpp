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

  cells.SetAsUnoccupied(m_cellPosition.Current());
  cells.SetAsUnoccupied(m_cellPosition.Next());

  auto angleToTarget = static_cast<int>(direct2d::GetAngleBetweenPoints(m_position, target));
  auto adjustedAngleToTarget = angleToTarget + 45;
  auto normalizedAngleToTarget = adjustedAngleToTarget > 359 ? adjustedAngleToTarget - 360 : adjustedAngleToTarget;
  auto segmentedAngleToTarget = normalizedAngleToTarget / 90;

  auto moveDirection = object_cell_position::move_direction::none;

  switch( segmentedAngleToTarget )
  {
    case 0:
      moveDirection = object_cell_position::move_direction::up;
      break;
    case 1:
      moveDirection = object_cell_position::move_direction::right;
      break;
    case 2:
      moveDirection = object_cell_position::move_direction::down;
      break;
    case 3:
      moveDirection = object_cell_position::move_direction::left;
      break;
  }

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

auto enemy_type_1::PreErase(level_cell_collection& cells) const noexcept -> void
{
  cells.SetAsUnoccupied(cells.CellId(m_position));
  if( m_destination ) cells.SetAsUnoccupied(*m_destination);
}
