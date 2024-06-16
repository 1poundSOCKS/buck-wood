#include "pch.h"
#include "enemy_type_3.h"

enemy_type_3::enemy_type_3(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) :
  enemy_object { position, scale, angle }, m_reloadTimer { 3.0f }
{
}

auto enemy_type_3::Update(float interval) noexcept -> void
{
  m_target ? Update(interval, *m_target) : base_object::Update(interval);
}

auto enemy_type_3::Update(float interval, POINT_2F targetPosition) noexcept -> void
{
  base_object::Update(interval);
  m_angle = DirectionTo(targetPosition);
  m_reloaded = m_reloadTimer.Update(interval);
}

auto enemy_type_3::CanShootAt(POINT_2F position) const -> bool
{
  return m_reloaded;
}

auto enemy_type_3::SetTarget(POINT_2F value) noexcept -> void
{
  m_target = value;
}
