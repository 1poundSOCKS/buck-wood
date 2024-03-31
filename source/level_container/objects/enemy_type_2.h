#pragma once

#include "enemy_type.h"
#include "enemy_movement_random.h"

class enemy_type_2 : public base_object
{

public:

  enum class state_type { moving, shooting };

  enemy_type_2(POINT_2F position);
  enemy_type_2(POINT_2F position, float reloadTime, int hitpoints);

  auto Update(float interval) -> void;

  [[nodiscard]] auto State() const -> state_type;
  [[nodiscard]] auto Destination() const -> std::optional<POINT_2F>;
  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;
  [[nodiscard]] auto Health() const -> float;

  auto ApplyDamage(int value) -> void;

private:

  state_type m_state { state_type::moving };
  constexpr static int m_movesBeforeStateChange { 3 };
  constexpr static float m_speed = 600;
  inline static std::uniform_int_distribution<int> m_positionDist { -10, 10 };

  std::optional<POINT_2F> m_destination;
  int m_maxHitpoints { 5 };
  int m_hitpoints { 5 };
  reload_timer m_reloadTimer;
  bool m_reloaded { false };
  constexpr static int m_shotsBeforeStateChange { 5 };

  int m_actionCount { 0 };

};

inline [[nodiscard]] auto enemy_type_2::State() const -> state_type
{
  return m_state;
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
  return m_reloaded;
}

inline [[nodiscard]] auto enemy_type_2::Reloaded() const -> bool
{
  return m_reloaded;
}

inline auto enemy_type_2::ApplyDamage(int value) -> void
{
  m_hitpoints = std::max(0, m_hitpoints - value);
  m_destroyed = m_hitpoints == 0;
}
