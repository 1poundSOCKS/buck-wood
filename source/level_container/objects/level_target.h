#pragma once

#include "base_object.h"
#include "play_event.h"
#include "reload_timer.h"
#include "mine.h"

class level_target : public base_object
{

public:

  level_target(POINT_2F position, float reloadTime, int hitpoints);

  auto Update(float interval) -> void;

  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;
  [[nodiscard]] auto MineType() -> mine::type;
  [[nodiscard]] auto Health() const -> float;

  auto ApplyDamage(int value) -> void;

private:

  enum class object_state { moving, shooting };

  object_state m_state { object_state::moving };
  float m_reloadTime;
  reload_timer m_reloadTimer;
  bool m_reloaded { false };
  int m_maxHitpoints;
  int m_hitpoints;
  std::optional<POINT_2F> m_destination;

  inline static std::uniform_int_distribution<int> m_positionDist { -10, 10 };
  inline static std::uniform_int_distribution<int> m_mineTypeDist { 1, 10 };
  inline static std::uniform_int_distribution<int> m_mineSpeed { 200, 400 };

};

inline [[nodiscard]] auto level_target::CanShootAt(POINT_2F position) const -> bool
{
  return m_reloaded && direct2d::GetDistanceBetweenPoints(m_position, position) < 1500;
}

inline [[nodiscard]] auto level_target::Reloaded() const -> bool
{
  return m_reloaded;
}

inline auto level_target::ApplyDamage(int value) -> void
{
  m_hitpoints = std::max(0, m_hitpoints - value);
  m_destroyed = m_hitpoints == 0;
}

inline [[nodiscard]] auto level_target::MineType() -> mine::type
{
  int mineTypeValue = m_mineTypeDist(pseudo_random_generator::get());
  return mineTypeValue > 5 ? mine::type::two : mine::type::one;
}

inline [[nodiscard]] auto level_target::Health() const -> float
{
  return static_cast<float>(m_hitpoints) / static_cast<float>(m_maxHitpoints);
}
