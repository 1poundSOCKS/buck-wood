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

auto level_target::Update(float interval) -> void
{
  base_object::Update(interval);

  float speed = 300;

  auto x = static_cast<float>(m_positionDist(pseudo_random_generator::get()));
  auto y = static_cast<float>(m_positionDist(pseudo_random_generator::get()));

  m_destination = m_destination ? m_destination : std::optional<POINT_2F>({x * 100, y * 100});
  m_position = m_destination ? direct2d::MoveTowards(m_position, *m_destination, speed * interval) : m_position;
  m_destination = m_destination && direct2d::AreEqual(m_position, *m_destination) ? std::nullopt : m_destination;
  m_reloaded = m_reloadTimer.Update(interval);
}
