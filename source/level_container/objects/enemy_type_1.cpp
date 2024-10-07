#include "pch.h"
#include "enemy_type_1.h"
#include "adjacent_floor_cells.h"

enemy_type_1::enemy_type_1(POINT_2F position, SCALE_2F scale, float angle) : enemy_object{position, scale, angle}
{
}

auto enemy_type_1::Update(float interval, POINT_2F target, level_cell_collection& cells) -> void
{
  auto angleToTarget = direct2d::GetAngleBetweenPoints(m_position, target);
  enemy_object::Update(interval, angleToTarget, cells);
}

[[nodiscard]] auto enemy_type_1::CanShootAt(POINT_2F position) const -> bool
{
  return false;
}

[[nodiscard]] auto enemy_type_1::Reloaded() const -> bool
{
  return false;
}
