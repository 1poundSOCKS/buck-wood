#include "pch.h"
#include "enemy_ship.h"

auto enemy_ship::Update(float interval, POINT_2F target) -> void
{
  enemy_object::Update(interval);

  switch( m_status )
  {
    case status::moving:
      m_status = UpdateWhenMoving(interval);
      break;
    case status::waiting:
      m_status = UpdateWhenWaiting(interval);
      break;
  }

  m_reloaded = m_reloadTimer ? m_reloadTimer->Update(interval) : false;

  switch( m_type )
  {
    case type::turret:
      m_angle = DirectionTo(target);
      break;
  }
}

auto enemy_ship::Type() const noexcept -> type
{
  return m_type;
}

[[nodiscard]] auto enemy_ship::CanShootAt(POINT_2F position) const -> bool
{
  return m_fireStatus == fire_status::enabled && m_reloaded;
}

[[nodiscard]] auto enemy_ship::Reloaded() const -> bool
{
  return m_reloaded;
}

auto enemy_ship::SetFireStatus(fire_status value) noexcept -> void
{
  m_fireStatus = value;
}

[[nodiscard]] auto enemy_ship::UpdateWhenMoving(float interval) noexcept -> status
{
  enemy_object::Update(interval);
  m_position = std::visit([this, interval](auto&& controller) { return controller(m_position, m_speed, interval); }, m_controller);
  return status::moving;
}

[[nodiscard]] auto enemy_ship::UpdateWhenWaiting(float interval) noexcept -> status
{
  enemy_object::Update(interval);
  auto finishedWaiting = m_waitTimer.Update(interval);
  return finishedWaiting ? status::moving : status::waiting;
}
