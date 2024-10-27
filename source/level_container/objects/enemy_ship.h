#pragma once

#include "reload_timer.h"
#include "enemy_object.h"
#include "enemy_fixed.h"
#include "enemy_path.h"
#include "enemy_area.h"

class enemy_ship : public enemy_object
{

public:

  enum class type { stalker, random, turret, guard };
  using controller = std::variant<enemy_fixed, enemy_path, enemy_area>;

  enemy_ship(POINT_2F position, SCALE_2F scale, float angle, type enemyType, float speed, controller enemyController);

  auto Update(float interval, POINT_2F target) -> void;

  [[nodiscard]] auto Type() const noexcept -> type;
  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;

private:

  enum class status { moving, waiting };

  [[nodiscard]] auto UpdateWhenMoving(float interval) noexcept -> status;
  [[nodiscard]] auto UpdateWhenWaiting(float interval) noexcept -> status;

private:

  inline static constexpr float m_reloadTime { 4.0f };
  inline static std::uniform_real_distribution<float> m_reloadTimerInitializer { 0.0f, 1.0f };

  type m_type;
  float m_speed;
  status m_status;
  reload_timer m_waitTimer;
  std::optional<reload_timer> m_reloadTimer;
  bool m_reloaded { false };
  controller m_controller;

};

inline enemy_ship::enemy_ship(POINT_2F position, SCALE_2F scale, float angle, type enemyType, float speed, controller enemyController) : 
  enemy_object{position, scale, angle, 1}, 
  m_type { enemyType },
  m_speed { speed },
  m_status { status::moving }, 
  m_waitTimer { 0.5f },
  m_reloaded { false },
  m_controller { enemyController }
{
  switch( enemyType )
  {
    case type::stalker:
      m_maxHitpoints = m_hitpoints = 5;
      m_reloadTimer = reload_timer { m_reloadTime, m_reloadTime * m_reloadTimerInitializer(pseudo_random_generator::get()) };
      break;

    case type::random:
      m_maxHitpoints = m_hitpoints = 3;
      break;

    case type::turret:
      m_maxHitpoints = m_hitpoints = 10;
      m_reloadTimer = reload_timer { m_reloadTime, m_reloadTime * m_reloadTimerInitializer(pseudo_random_generator::get()) };
      break;
  }
}
