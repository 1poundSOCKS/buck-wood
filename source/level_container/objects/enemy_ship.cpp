#include "pch.h"
#include "enemy_ship.h"
#include "adjacent_floor_cells.h"

auto enemy_ship::Update(float interval, POINT_2F target, level_cell_collection& cells) -> void
{
  switch( m_type )
  {
    case type::stalker:
      UpdateStalker(interval, target, cells);
      break;
    
    case type::random:
      UpdateRandom(interval, target, cells);
      break;

    case type::turret:
      UpdateTurret(interval, target);
      break;
  }
}

auto enemy_ship::Type() const noexcept -> type
{
  return m_type;
}

[[nodiscard]] auto enemy_ship::CanShootAt(POINT_2F position) const -> bool
{
  return m_reloaded;
}

[[nodiscard]] auto enemy_ship::Reloaded() const -> bool
{
  return m_reloaded;
}

auto enemy_ship::UpdateStalker(float interval, POINT_2F target, level_cell_collection &cells) -> void
{
  auto angleToTarget = direct2d::GetAngleBetweenPoints(m_position, target);
  enemy_object::Update(interval, angleToTarget, cells);
  m_position = m_path(m_position, interval);
}

auto enemy_ship::UpdateRandom(float interval, POINT_2F targetPosition, level_cell_collection& cells) -> void
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

auto enemy_ship::UpdateTurret(float interval, POINT_2F targetPosition) noexcept -> void
{
  base_object::Update(interval);
  m_angle = DirectionTo(targetPosition);
  m_reloaded = m_reloadTimer.Update(interval);
}

[[nodiscard]] auto enemy_ship::UpdateWhenMoving(float interval, level_cell_collection& cells) noexcept -> status
{
  std::uniform_int_distribution angle { 0, 359 };
  auto moveAngle = static_cast<float>(angle(pseudo_random_generator::get()));
  auto direction = object_cell_position::MoveDirection(moveAngle);
  enemy_object::Update(interval, direction, cells);
  m_waitTimer.Reset();
  return status::moving;
}

[[nodiscard]] auto enemy_ship::UpdateWhenWaiting(float interval) noexcept -> status
{
  base_object::Update(interval);
  auto finishedWaiting = m_waitTimer.Update(interval);
  return finishedWaiting ? status::moving : status::waiting;
}
