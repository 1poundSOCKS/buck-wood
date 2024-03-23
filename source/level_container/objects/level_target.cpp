#include "pch.h"
#include "level_target.h"
#include "renderers.h"
#include "shape_generator.h"

level_target::level_target(POINT_2F position, float reloadTime, int hitpoints) : base_object { position, { 0, 0 }, { 1.5f, 1.5f }, 0 },
  m_reloadTimer { reloadTime }, m_maxHitpoints { hitpoints }, m_hitpoints { hitpoints }
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

auto level_target::Activate() -> void
{
  m_activated = true;
}

[[nodiscard]] auto level_target::Reloaded() const -> bool
{
  return m_reloaded;
}

auto level_target::Update(float interval, std::optional<POINT_2F> playerPosition) -> void
{
  Update(interval, playerPosition, std::ranges::empty_view<player_bullet>());
}
