#pragma once

#include "framework.h"
#include "base_object.h"
// #include "object_destination.h"
#include "reload_timer.h"

template <typename movement_type> class enemy_type : public base_object
{

public:

  enemy_type(POINT_2F position);
  enemy_type(POINT_2F position, float reloadTime, int hitpoints);
  auto Update(float interval) -> void;

  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;
  [[nodiscard]] auto Health() const -> float;

  auto ApplyDamage(int value) -> void;

private:

  movement_type m_movement;
  int m_maxHitpoints { 1 };
  int m_hitpoints { 1 };
  reload_timer m_reloadTimer { 1.0f };
  bool m_reloaded { false };

};

template <typename movement_type> enemy_type<movement_type>::enemy_type(POINT_2F position) : base_object { position, { 1, 1 }, 0 }
{
}

template <typename movement_type> enemy_type<movement_type>::enemy_type(POINT_2F position, float reloadTime, int hitpoints) : base_object { position, { 1, 1 }, 0 }, m_reloadTimer { reloadTime }, m_hitpoints { hitpoints }
{
}

template <typename movement_type> auto enemy_type<movement_type>::Update(float interval) -> void
{
  base_object::Update(interval);
  m_position = m_movement(m_position, interval);
  m_reloaded = m_reloadTimer.Update(interval);
}

template <typename movement_type> [[nodiscard]] auto enemy_type<movement_type>::Health() const -> float
{
  return static_cast<float>(m_hitpoints) / static_cast<float>(m_maxHitpoints);
}

template <typename movement_type> [[nodiscard]] auto enemy_type<movement_type>::CanShootAt(POINT_2F position) const -> bool
{
  return m_reloaded && direct2d::GetDistanceBetweenPoints(m_position, position) < 1500;
}

template <typename movement_type> [[nodiscard]] auto enemy_type<movement_type>::Reloaded() const -> bool
{
  return m_reloaded;
}

template <typename movement_type> auto enemy_type<movement_type>::ApplyDamage(int value) -> void
{
  m_hitpoints = std::max(0, m_hitpoints - value);
  m_destroyed = m_hitpoints == 0;
}
