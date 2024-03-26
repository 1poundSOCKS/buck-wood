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

  [[nodiscard]] auto IsActivated() const -> bool;
  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;

  auto Activate() -> void;
  auto SetPlayerPosition(float x, float y) -> void;
  [[nodiscard]] auto Reloaded() const -> bool;

  auto ApplyDamage(int value) -> void
  {
    m_hitpoints = std::max(0, m_hitpoints - value);
    m_destroyed = m_hitpoints == 0;
  }

  [[nodiscard]] auto MineType() -> mine::type
  {
    int mineTypeValue = m_mineTypeDist(pseudo_random_generator::get());
    return mineTypeValue > 5 ? mine::type::two : mine::type::one;
  }

  [[nodiscard]] auto Health() const -> float
  {
    return static_cast<float>(m_hitpoints) / static_cast<float>(m_maxHitpoints);
  }

private:

  float m_reloadTime;
  bool m_activated = false;
  reload_timer m_reloadTimer;
  bool m_reloaded { false };
  int m_maxHitpoints;
  int m_hitpoints;
  std::optional<POINT_2F> m_destination;

  inline static std::uniform_int_distribution<int> m_positionDist { -10, 10 };
  inline static std::uniform_int_distribution<int> m_mineTypeDist { 1, 10 };
  inline static std::uniform_int_distribution<int> m_mineSpeed { 200, 400 };

};
