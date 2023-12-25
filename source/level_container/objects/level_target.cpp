#include "pch.h"
#include "level_target.h"
#include "renderers.h"
#include "shape_generator.h"

level_target::level_target(float x, float y) : m_position { x, y }
{
}

[[nodiscard]] auto level_target::IsActivated() const -> bool
{
  return m_activated;
}

[[nodiscard]] auto level_target::CanShootAt(game_point position) const -> bool
{
  return !m_activated && m_reloaded && m_position.DistanceTo(position) < 1500;
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
