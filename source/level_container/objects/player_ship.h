#pragma once

#include "framework.h"
#include "base_object.h"
#include "object_velocity.h"
#include "play_event.h"
#include "reload_timer.h"
#include "reload_counter.h"
#include "health_status.h"
#include "level_cell_collection.h"
#include "adjacent_cell_visitor.h"

class player_ship : public base_object
{

public:

  using points_collection = std::vector<D2D1_POINT_2F>;

public:

  player_ship(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity);

  auto Update(float interval, const level_cell_collection& cells) -> void;

  auto Rotate(float angle) -> void;
  auto ApplyDamage(int value) -> void;
  auto ApplyFatalDamage() -> void;

  [[nodiscard]] auto Velocity() const noexcept -> VELOCITY_2F;
  [[nodiscard]] auto ThrusterOn() const -> bool;
  [[nodiscard]] auto TriggerDown() const -> bool;
  [[nodiscard]] auto ShieldStatus() const -> const health_status&;
  [[nodiscard]] auto CanShoot() -> bool;
  [[nodiscard]] auto EmitThrustParticle() -> bool;
  [[nodiscard]] auto ThrustParticleDirection() const -> float;
  [[nodiscard]] auto ShootAngle() const noexcept -> float;

private:

  auto UpdateWhenActive(float interval, const level_cell_collection& cells) -> void;
  auto UpdateWhenCelebrating(float interval) -> void;

private:

  static constexpr float m_thrustPower { 2000.0f };
  static constexpr float m_friction { 0.95f };
  static constexpr SIZE_F m_objectSize { 90, 90 };

private:

  object_velocity m_velocity;
  health_status m_shieldStatus { 10 };
  bool m_triggerDown { false };
  reload_counter m_playerReloadCounter { 1.0f / 3.0f, 1 };
  reload_counter m_thrustEmmisionCounter { 1.0f / 10.0f, 2 };
  float m_shootAngle;
  std::optional<POINT_2F> m_leftThumbstickPosition;

};

inline auto player_ship::Rotate(float angle) -> void
{
  m_angle = direct2d::RotateAngle(m_angle, angle);
}

inline auto player_ship::ApplyDamage(int value) -> void
{
  if( m_shieldStatus.ApplyDamage(value) == 0 )
  {
    m_destroyed = true;
  }
}

inline auto player_ship::ApplyFatalDamage() -> void
{
  m_shieldStatus.ApplyFatalDamage();
  m_destroyed = true;
}

inline [[nodiscard]] auto player_ship::Velocity() const noexcept -> VELOCITY_2F
{
  return m_velocity.Get();
}

inline [[nodiscard]] auto player_ship::ThrusterOn() const -> bool
{
  return !m_destroyed && m_leftThumbstickPosition;
}

inline [[nodiscard]] auto player_ship::TriggerDown() const -> bool
{
  return !m_destroyed && m_triggerDown;
}

inline [[nodiscard]] auto player_ship::ShieldStatus() const -> const health_status&
{
  return m_shieldStatus;
}

inline [[nodiscard]] auto player_ship::CanShoot() -> bool
{
  return !m_destroyed && TriggerDown() && m_playerReloadCounter.Get(1, true) == 1;
}

inline [[nodiscard]] auto player_ship::ThrustParticleDirection() const -> float
{
  auto angle = m_leftThumbstickPosition ? direct2d::CalculateDirection({m_leftThumbstickPosition->x, m_leftThumbstickPosition->y}) : 0;
  return direct2d::RotateAngle(angle, 180);
}

inline [[nodiscard]] auto player_ship::EmitThrustParticle() -> bool
{
  return !m_destroyed && m_leftThumbstickPosition && m_thrustEmmisionCounter.Get(1, true) == 1;
}

inline auto player_ship::ShootAngle() const noexcept -> float
{
  return m_shootAngle;
}
