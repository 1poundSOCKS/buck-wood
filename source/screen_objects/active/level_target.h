#pragma once

#include "play_event.h"
#include "geometry.h"
#include "path_geometry.h"
#include "reload_timer.h"

class level_target
{
public:

  level_target(float x, float y);

  [[nodiscard]] auto Position() const -> game_point;
  [[nodiscard]] auto IsActivated() const -> bool;
  [[nodiscard]] auto Geometry() const -> const path_geometry&;
  [[nodiscard]] auto ShootAt(game_point position) const -> bool;

  auto HitByBullet() -> void;
  auto Activate() -> void;
  auto SetPlayerPosition(float x, float y) -> void;
  auto Update(float interval) -> void;
  [[nodiscard]] auto Reloaded() const -> bool;
  
private:

  inline static const float m_defaultReloadTime { 4 };

  game_point m_position { 0, 0 };
  bool m_activated = false;
  path_geometry m_geometry { d2d_factory::get_raw() };
  reload_timer m_reloadTimer;
  bool m_reloaded { false };
  int m_hitPoints { 10 };

};
