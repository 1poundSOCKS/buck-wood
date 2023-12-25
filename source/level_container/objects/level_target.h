#pragma once

#include "play_event.h"
#include "path_geometry.h"
#include "reload_timer.h"
#include "game_scale.h"

class level_target
{

public:

  level_target(float x, float y);

  [[nodiscard]] auto Scale() const -> game_scale;
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> const game_point&;
  auto Update(float interval) -> void;

  [[nodiscard]] auto IsActivated() const -> bool;
  [[nodiscard]] auto CanShootAt(game_point position) const -> bool;

  auto HitByBullet() -> void;
  auto Activate() -> void;
  auto SetPlayerPosition(float x, float y) -> void;
  [[nodiscard]] auto Reloaded() const -> bool;
  
private:

  inline static constexpr float m_defaultReloadTime { 4 };

  game_point m_position { 0, 0 };
  bool m_activated = false;
  reload_timer m_reloadTimer { m_defaultReloadTime };
  bool m_reloaded { false };
  int m_hitPoints { 10 };

};

inline [[nodiscard]] auto level_target::Scale() const -> game_scale
{
  return game_scale { 1.0f, 1.0f };
}

inline [[nodiscard]] auto level_target::Angle() const -> float
{
  return 0;
}

inline [[nodiscard]] auto level_target::Position() const -> const game_point&
{
  return m_position;
}
