#pragma once

#include "base_object.h"
#include "reload_timer.h"
#include "enemy_bullet_1.h"
#include "enemy_movement_random.h"

class enemy_type_2 : public base_object
{

public:

  enemy_type_2(POINT_2F position, float reloadTime, int hitpoints);

  auto Update(float interval) -> void;

  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;
  [[nodiscard]] auto BulletType() -> enemy_bullet_1::type;
  [[nodiscard]] auto Health() const -> float;

  auto ApplyDamage(int value) -> void;

private:

  inline static std::uniform_int_distribution<int> m_bulletType { 1, 10 };
  inline static std::uniform_int_distribution<int> m_bulletSpeed { 200, 400 };

  enum class object_state { moving, shooting };

  object_state m_state { object_state::moving };
  float m_reloadTime;
  reload_timer m_reloadTimer;
  bool m_reloaded { false };
  int m_maxHitpoints;
  int m_hitpoints;

  enemy_movement_random m_randomMovement;

};

inline [[nodiscard]] auto enemy_type_2::CanShootAt(POINT_2F position) const -> bool
{
  return m_reloaded && direct2d::GetDistanceBetweenPoints(m_position, position) < 1500;
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

inline [[nodiscard]] auto enemy_type_2::BulletType() -> enemy_bullet_1::type
{
  int enemy_bullet_1TypeValue = m_bulletType(pseudo_random_generator::get());
  return enemy_bullet_1TypeValue > 5 ? enemy_bullet_1::type::two : enemy_bullet_1::type::one;
}

inline [[nodiscard]] auto enemy_type_2::Health() const -> float
{
  return static_cast<float>(m_hitpoints) / static_cast<float>(m_maxHitpoints);
}
