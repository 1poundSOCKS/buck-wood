#include "pch.h"
#include "enemy_type_2.h"
#include "adjacent_floor_cells.h"

enemy_type_2::enemy_type_2(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : 
  enemy_object { position, scale, angle }, m_status { status::moving }, 
  m_waitTimer { 0.5f }, m_reloadTimer { 5.0f }
{
}

auto enemy_type_2::Update(float interval, POINT_2F targetPosition, level_cell_collection& cells) -> void
{
  switch( m_status )
  {
    case status::moving:
      m_status = UpdateWhenMoving(interval, cells);
      break;
    case status::waiting:
      m_status = UpdateWhenWaiting(interval);
      break;
  }

  m_reloaded = m_reloadTimer.Update(interval);
}

[[nodiscard]] auto enemy_type_2::UpdateWhenMoving(float interval, level_cell_collection& cells) noexcept -> status
{
  std::uniform_int_distribution angle { 0, 359 };
  auto moveAngle = static_cast<float>(angle(pseudo_random_generator::get()));
  auto direction = object_cell_position::MoveDirection(moveAngle);
  enemy_object::Update(interval, direction, cells);
  m_waitTimer.Reset();
  return status::moving;
}

[[nodiscard]] auto enemy_type_2::UpdateWhenWaiting(float interval) noexcept -> status
{
  base_object::Update(interval);
  auto finishedWaiting = m_waitTimer.Update(interval);
  return finishedWaiting ? status::moving : status::waiting;
}
