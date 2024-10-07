#include "pch.h"
#include "enemy_type_1.h"
#include "adjacent_floor_cells.h"

enemy_type_1::enemy_type_1(POINT_2F position, SCALE_2F scale, float angle, type enemyType) : 
  enemy_object{position, scale, angle}, 
  m_type { enemyType },
  m_status { status::moving }, 
  m_waitTimer { 0.5f }, 
  m_reloadTimer { 5.0f },
  m_reloaded { false }
{
}

auto enemy_type_1::Update(float interval, POINT_2F target, level_cell_collection& cells) -> void
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

auto enemy_type_1::Type() const noexcept -> type
{
  return m_type;
}

[[nodiscard]] auto enemy_type_1::CanShootAt(POINT_2F position) const -> bool
{
  return m_reloaded;
}

[[nodiscard]] auto enemy_type_1::Reloaded() const -> bool
{
  return m_reloaded;
}

auto enemy_type_1::UpdateStalker(float interval, POINT_2F target, level_cell_collection &cells) -> void
{
  auto angleToTarget = direct2d::GetAngleBetweenPoints(m_position, target);
  enemy_object::Update(interval, angleToTarget, cells);
}

auto enemy_type_1::UpdateRandom(float interval, POINT_2F targetPosition, level_cell_collection& cells) -> void
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

auto enemy_type_1::UpdateTurret(float interval, POINT_2F targetPosition) noexcept -> void
{
  base_object::Update(interval);
  m_angle = DirectionTo(targetPosition);
  m_reloaded = m_reloadTimer.Update(interval);
}

[[nodiscard]] auto enemy_type_1::UpdateWhenMoving(float interval, level_cell_collection& cells) noexcept -> status
{
  std::uniform_int_distribution angle { 0, 359 };
  auto moveAngle = static_cast<float>(angle(pseudo_random_generator::get()));
  auto direction = object_cell_position::MoveDirection(moveAngle);
  enemy_object::Update(interval, direction, cells);
  m_waitTimer.Reset();
  return status::moving;
}

[[nodiscard]] auto enemy_type_1::UpdateWhenWaiting(float interval) noexcept -> status
{
  base_object::Update(interval);
  auto finishedWaiting = m_waitTimer.Update(interval);
  return finishedWaiting ? status::moving : status::waiting;
}
