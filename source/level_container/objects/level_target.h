#pragma once

#include "play_event.h"
#include "reload_timer.h"

class level_target
{

public:

  level_target(POINT_2F position, float reloadTime, int hitpoints);

  [[nodiscard]] auto Scale() const -> SCALE_2F;
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> POINT_2F;
  [[nodiscard]] auto Destroyed() const -> bool;
  auto Update(float interval, std::optional<POINT_2F> playerPosition) -> void;
  auto Destroy() -> void;

  [[nodiscard]] auto IsActivated() const -> bool;
  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;

  auto HitByBullet() -> void;
  auto Activate() -> void;
  auto SetPlayerPosition(float x, float y) -> void;
  [[nodiscard]] auto Reloaded() const -> bool;

  auto ApplyDamage(int value) -> void
  {
    m_hitpoints = std::max(0, m_hitpoints - value);
    m_destroyed = m_hitpoints == 0;
  }

private:

  float m_reloadTime;
  D2D1_POINT_2F m_position;
  float m_angle { 0 };
  bool m_activated = false;
  reload_timer m_reloadTimer;
  bool m_reloaded { false };
  int m_hitPoints { 10 };
  bool m_destroyed { false };
  int m_hitpoints;

};

inline [[nodiscard]] auto level_target::Scale() const -> SCALE_2F
{
  return { 1.5f, 1.5f };
}

inline [[nodiscard]] auto level_target::Angle() const -> float
{
  return m_angle;
}

inline [[nodiscard]] auto level_target::Position() const -> POINT_2F
{
  return m_position;
}

inline [[nodiscard]] auto level_target::Destroyed() const -> bool
{
  return m_destroyed;
}

inline auto level_target::Destroy() -> void
{
  m_destroyed = true;
}
