#pragma once

#include "play_event.h"
#include "reload_timer.h"

class level_target
{

public:

  level_target(POINT_2F position, float reloadTime);

  [[nodiscard]] auto Scale() const -> SCALE_2F;
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> POINT_2F;
  constexpr [[nodiscard]] auto Destroyed() const -> bool { return false; }
  auto Update(float interval, std::optional<POINT_2F> playerPosition) -> void;

  [[nodiscard]] auto IsActivated() const -> bool;
  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;

  auto HitByBullet() -> void;
  auto Activate() -> void;
  auto SetPlayerPosition(float x, float y) -> void;
  [[nodiscard]] auto Reloaded() const -> bool;

private:

  float m_reloadTime;
  D2D1_POINT_2F m_position;
  float m_angle { 0 };
  bool m_activated = false;
  reload_timer m_reloadTimer;
  bool m_reloaded { false };
  int m_hitPoints { 10 };

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
