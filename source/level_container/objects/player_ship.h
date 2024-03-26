#pragma once

#include "framework.h"
#include "base_object.h"
#include "object_velocity.h"
#include "play_event.h"
#include "reload_timer.h"
#include "reload_counter.h"
#include "health_status.h"

class player_ship : public base_object, public object_velocity
{

public:

  using points_collection = std::vector<D2D1_POINT_2F>;
  enum class fire_mode { none, one, two };

public:

  enum class movement_type { horizontal, both };

  player_ship(movement_type movementType, POINT_2F position);

  auto Update(float interval) -> void;
  auto Update(float interval, bool enableControl) -> void;

  auto Rotate(float angle) -> void;
  auto SetThrust(float value) -> void;
  auto ApplyDamage(int value) -> void;
  auto ApplyFatalDamage() -> void;
  auto SetActive(bool value) -> void;
  auto SetInvulnerable(bool value) -> void;

  [[nodiscard]] auto ThrusterOn() const -> bool;
  [[nodiscard]] auto TriggerDown() const -> bool;
  [[nodiscard]] auto ShieldStatus() const -> const health_status&;
  [[nodiscard]] auto ShieldsUp() const -> bool;
  [[nodiscard]] auto CanShoot() -> bool;
  [[nodiscard]] auto EmitThrustParticle() -> bool;
  [[nodiscard]] auto FireMode() const -> fire_mode;

private:

  auto UpdateWithControl(float interval, bool enableControl) -> void;
  auto UpdateWithoutControl(float interval, bool enableControl) -> void;
  auto UpdateWithHorizontalControl(float interval, bool enableControl) -> void;
  auto UpdateWithAllControl(float interval, bool enableControl) -> void;
  
  static [[nodiscard]] auto GetUpdatedAngle(D2D1_POINT_2F position, float direction, D2D1_POINT_2F destination, float interval) -> float;
  static [[nodiscard]] auto GetUpdatedPosition(D2D1_POINT_2F position, VELOCITY_2F velocity, float interval) -> D2D1_POINT_2F;
  static [[nodiscard]] auto SwitchFireMode(fire_mode fireMode) -> fire_mode;

private:

  static constexpr float m_thrustPower { 500.0f };

private:

  movement_type m_movementType;
  bool m_thrusterOn { false };
  health_status m_shieldStatus { 10 };
  bool m_active { false };
  bool m_invulnerable { false };
  
  float m_thrust { 0 };
  bool m_triggerDown { false };
  bool m_shieldsUp { false };

  reload_counter m_playerReloadCounter { 1.0f / 3.0f, 1 };
  fire_mode m_fireMode { fire_mode::one };

  reload_counter m_thrustEmmisionCounter { 1.0f / 10.0f, 1 };

  std::optional<D2D1_POINT_2F> m_destination;

};

inline auto player_ship::Rotate(float angle) -> void
{
  m_angle = direct2d::RotateAngle(m_angle, angle);
}

inline auto player_ship::SetThrust(float value) -> void
{
  m_thrust = value * m_thrustPower;
}

inline auto player_ship::ApplyDamage(int value) -> void
{
  if( !m_shieldsUp && m_shieldStatus.ApplyDamage(value) == 0 )
  {
    m_destroyed = true;
  }
}

inline auto player_ship::ApplyFatalDamage() -> void
{
  m_shieldStatus.ApplyFatalDamage();
  m_destroyed = true;
}

inline auto player_ship::SetActive(bool value) -> void
{
  m_active = value;
}

inline auto player_ship::SetInvulnerable(bool value) -> void
{
  m_invulnerable = value;
}

inline [[nodiscard]] auto player_ship::ThrusterOn() const -> bool
{
  return !m_destroyed && m_thrusterOn;
}

inline [[nodiscard]] auto player_ship::TriggerDown() const -> bool
{
  return !m_destroyed && m_triggerDown;
}

inline [[nodiscard]] auto player_ship::ShieldStatus() const -> const health_status&
{
  return m_shieldStatus;
}

inline [[nodiscard]] auto player_ship::ShieldsUp() const -> bool
{
  return !m_destroyed && m_shieldsUp;
}

inline [[nodiscard]] auto player_ship::CanShoot() -> bool
{
  return !m_destroyed && m_triggerDown && !m_shieldsUp && m_playerReloadCounter.Get(1, true) == 1;
}

inline [[nodiscard]] auto player_ship::EmitThrustParticle() -> bool
{
  return !m_destroyed && m_thrusterOn && m_thrustEmmisionCounter.Get(1, true) == 1;
}

inline [[nodiscard]] auto player_ship::FireMode() const -> fire_mode
{
  return m_fireMode;
}

inline [[nodiscard]] auto player_ship::SwitchFireMode(fire_mode fireMode) -> fire_mode
{
  switch( fireMode )
  {
    case fire_mode::one:
      return fire_mode::two;
    case fire_mode::two:
      return fire_mode::one;
    default:
      return fire_mode::none;
  }
}
