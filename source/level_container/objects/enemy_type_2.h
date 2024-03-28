#pragma once

#include "enemy_type.h"
#include "enemy_movement_random.h"

class enemy_type_2 : public base_object
{

public:

  enemy_type_2(POINT_2F position);
  enemy_type_2(POINT_2F position, float reloadTime, int hitpoints);

  auto Update(float interval) -> void;

  [[nodiscard]] auto Destination() const -> std::optional<POINT_2F>;
  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;
  [[nodiscard]] auto Health() const -> float;

  auto SetDestination(std::optional<POINT_2F> destination) -> void;
  auto ApplyDamage(int value) -> void;

private:

  std::optional<POINT_2F> m_destination;
  int m_maxHitpoints { 5 };
  int m_hitpoints { 5 };
  reload_timer m_reloadTimer { 1.0f };
  bool m_reloaded { false };

};

inline enemy_type_2::enemy_type_2(POINT_2F position, float reloadTime, int hitpoints) : 
  base_object { position, { 1.5, 1.5 }, 0 }, m_reloadTimer { reloadTime }, m_hitpoints { hitpoints }
{
}

inline auto enemy_type_2::Update(float interval) -> void
{
  base_object::Update(interval);
  m_reloaded = m_reloadTimer.Update(interval);
}

inline [[nodiscard]] auto enemy_type_2::Destination() const -> std::optional<POINT_2F>
{
  return m_destination;
}

inline [[nodiscard]] auto enemy_type_2::Health() const -> float
{
  return static_cast<float>(m_hitpoints) / static_cast<float>(m_maxHitpoints);
}

inline [[nodiscard]] auto enemy_type_2::CanShootAt(POINT_2F position) const -> bool
{
  return m_reloaded && direct2d::GetDistanceBetweenPoints(m_position, position) < 1500;
}

inline [[nodiscard]] auto enemy_type_2::Reloaded() const -> bool
{
  return m_reloaded;
}

inline auto enemy_type_2::SetDestination(std::optional<POINT_2F> destination) -> void
{
  m_destination = destination;
}

inline auto enemy_type_2::ApplyDamage(int value) -> void
{
  m_hitpoints = std::max(0, m_hitpoints - value);
  m_destroyed = m_hitpoints == 0;
}
