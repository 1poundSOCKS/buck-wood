#include "pch.h"
#include "level_target.h"
#include "renderers.h"
#include "shape_generator.h"

level_target::level_target(POINT_2F position, float reloadTime) : m_position { position }, m_reloadTimer { reloadTime }
{
}

[[nodiscard]] auto level_target::IsActivated() const -> bool
{
  return m_activated;
}

[[nodiscard]] auto level_target::CanShootAt(POINT_2F position) const -> bool
{
  return !m_activated && m_reloaded && direct2d::GetDistanceBetweenPoints(m_position, position) < 1500;
}

auto level_target::HitByBullet() -> void
{
  if( --m_hitPoints <= 0 )
  {
    Activate();
  }
}

auto level_target::Activate() -> void
{
  m_activated = true;
}

auto level_target::Update(float interval, std::optional<POINT_2F> playerPosition) -> void
{
  m_angle = playerPosition ? direct2d::GetAngleBetweenPoints(m_position, *playerPosition) : m_angle;
  m_reloaded = m_reloadTimer.Update(interval);
}

[[nodiscard]] auto level_target::Reloaded() const -> bool
{
  return m_reloaded;
}
