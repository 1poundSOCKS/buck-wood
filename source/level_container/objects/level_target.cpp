#include "pch.h"
#include "level_target.h"
#include "renderers.h"
#include "shape_generator.h"

level_target::level_target(POINT_2F position, float reloadTime, int hitpoints) : base_object { position, { 1.5f, 1.5f }, 0 },
  m_reloadTimer { reloadTime }, m_maxHitpoints { hitpoints }, m_hitpoints { hitpoints }
{
}

auto level_target::Update(float interval) -> void
{
  base_object::Update(interval);

  float speed = 500;

  auto x = static_cast<float>(m_positionDist(pseudo_random_generator::get()));
  auto y = static_cast<float>(m_positionDist(pseudo_random_generator::get()));

  m_destination = m_destination ? m_destination : std::optional<POINT_2F>({x * 100, y * 100});
  m_position = m_destination ? direct2d::MoveTowards(m_position, *m_destination, speed * interval) : m_position;
  m_destination = m_destination && direct2d::AreEqual(m_position, *m_destination) ? std::nullopt : m_destination;
  m_reloaded = m_reloadTimer.Update(interval);
}
