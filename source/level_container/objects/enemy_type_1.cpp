#include "pch.h"
#include "enemy_type_1.h"
#include "adjacent_floor_cells.h"

enemy_type_1::enemy_type_1(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : enemy_object{position, scale, angle}
{
}

auto enemy_type_1::Update(float interval, POINT_2F target, level_cell_collection& cells) -> void
{
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

  enemy_object::Update(interval, moveDirection, cells);
}

[[nodiscard]] auto enemy_type_1::CanShootAt(POINT_2F position) const -> bool
{
  return false;
}

[[nodiscard]] auto enemy_type_1::Reloaded() const -> bool
{
  return false;
}
