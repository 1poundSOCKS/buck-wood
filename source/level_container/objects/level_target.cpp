#include "pch.h"
#include "level_target.h"
#include "renderers.h"
#include "shape_generator.h"

level_target::level_target(D2D1_POINT_2F position) : m_position { position }
{
}

[[nodiscard]] auto level_target::IsActivated() const -> bool
{
  return m_activated;
}

[[nodiscard]] auto level_target::CanShootAt(D2D1_POINT_2F position) const -> bool
{
  // return !m_activated && m_reloaded && m_position.DistanceTo(position) < 1500;
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

auto level_target::Update(float interval) -> void
{
  m_reloaded = m_reloadTimer.Update(interval);
}

[[nodiscard]] auto level_target::Reloaded() const -> bool
{
  return m_reloaded;
}
