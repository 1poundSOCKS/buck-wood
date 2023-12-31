#include "pch.h"
#include "level_target.h"
#include "renderers.h"
#include "shape_generator.h"

level_target::level_target(game_point position) : m_data { std::make_shared<data>(position) }
{
}

[[nodiscard]] auto level_target::IsActivated() const -> bool
{
  return m_data->m_activated;
}

[[nodiscard]] auto level_target::CanShootAt(game_point position) const -> bool
{
  return !m_data->m_activated && m_data->m_reloaded && m_data->m_position.DistanceTo(position) < 1500;
}

auto level_target::HitByBullet() -> void
{
  if( --m_data->m_hitPoints <= 0 )
  {
    Activate();
  }
}

auto level_target::Activate() -> void
{
  m_data->m_activated = true;
}

auto level_target::Update(float interval) -> void
{
  m_data->m_reloaded = m_data->m_reloadTimer.Update(interval);
}

[[nodiscard]] auto level_target::Reloaded() const -> bool
{
  return m_data->m_reloaded;
}
