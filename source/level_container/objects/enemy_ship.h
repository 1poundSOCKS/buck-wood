#pragma once

#include "reload_timer.h"
#include "enemy_object.h"
#include "enemy_path.h"

class enemy_ship : public enemy_object
{

public:

  enum class type { stalker, random, turret };

  enemy_ship(POINT_2F position, SCALE_2F scale, float angle, type enemyType, float speed, std::ranges::input_range auto&& points);

  auto Update(float interval, POINT_2F target) -> void;

  [[nodiscard]] auto Type() const noexcept -> type;
  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;

private:

  enum class status { moving, waiting };

  auto UpdateStalker(float interval, POINT_2F target) -> void;
  auto UpdateRandom(float interval, POINT_2F target) -> void;
  auto UpdateTurret(float interval, POINT_2F targetPosition) noexcept -> void;
  [[nodiscard]] auto UpdateWhenMoving(float interval) noexcept -> status;
  [[nodiscard]] auto UpdateWhenWaiting(float interval) noexcept -> status;

private:

  type m_type;
  float m_speed;
  status m_status;
  reload_timer m_waitTimer;
  reload_timer m_reloadTimer;
  bool m_reloaded { false };
  enemy_path m_path;

};

enemy_ship::enemy_ship(POINT_2F position, SCALE_2F scale, float angle, type enemyType, float speed, std::ranges::input_range auto&& points) : 
  enemy_object{position, scale, angle, 1}, 
  m_type { enemyType },
  m_speed { speed },
  m_status { status::moving }, 
  m_waitTimer { 0.5f }, 
  m_reloadTimer { 4.0f },
  m_reloaded { false },
  m_path { points }
{
  switch( enemyType )
  {
    case type::stalker:
      m_maxHitpoints = m_hitpoints = 5;
      break;

    case type::random:
      m_maxHitpoints = m_hitpoints = 3;
      break;

    case type::turret:
      m_maxHitpoints = m_hitpoints = 10;
      break;
  }
}
