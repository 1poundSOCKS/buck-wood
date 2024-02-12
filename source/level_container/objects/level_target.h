#pragma once

#include "play_event.h"
#include "reload_timer.h"

class level_target
{

public:

  level_target(D2D1_POINT_2F position, float reloadTime);

  [[nodiscard]] auto Scale() const -> SCALE_2F;
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> D2D1_POINT_2F;
  auto Update(float interval) -> void;

  constexpr [[nodiscard]] auto Destroyed() const -> bool { return false; }
  [[nodiscard]] auto IsActivated() const -> bool;
  [[nodiscard]] auto CanShootAt(D2D1_POINT_2F position) const -> bool;

  auto HitByBullet() -> void;
  auto Activate() -> void;
  auto SetPlayerPosition(float x, float y) -> void;
  [[nodiscard]] auto Reloaded() const -> bool;

private:

  float m_reloadTime;
  D2D1_POINT_2F m_position;
  bool m_activated = false;
  reload_timer m_reloadTimer;
  bool m_reloaded { false };
  int m_hitPoints { 10 };

};

inline [[nodiscard]] auto level_target::Scale() const -> SCALE_2F
{
  return { 1.0f, 1.0f };
}

inline [[nodiscard]] auto level_target::Angle() const -> float
{
  return 0;
}

inline [[nodiscard]] auto level_target::Position() const -> D2D1_POINT_2F
{
  return m_position;
}
