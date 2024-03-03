#pragma once

#include "framework.h"
#include "player_state.h"
#include "play_event.h"
#include "reload_timer.h"
#include "reload_counter.h"

class player_ship
{

public:

  using points_collection = std::vector<D2D1_POINT_2F>;
  enum class fire_mode { none, one, two };

public:

  player_ship(std::shared_ptr<player_state> state);

  [[nodiscard]] auto Scale() const -> SCALE_2F { return { 1.5f, 1.5f }; };
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> D2D1_POINT_2F;
  [[nodiscard]] auto Destroyed() const -> bool;
  auto Update(float interval) -> void;

  auto Update(float interval, bool enableControl) -> void;

  auto SetAngle(float angle) -> void;
  auto Rotate(float angle) -> void;
  auto SetThrust(float value) -> void;
  auto ApplyDamage(int value) -> void;
  auto ApplyFatalDamage() -> void;
  auto Destroy() -> void;
  auto SetPlayerActive(bool value) -> void;

  [[nodiscard]] auto PreviousPosition() const -> D2D1_POINT_2F;
  [[nodiscard]] auto Velocity() const -> VELOCITY_2F;
  [[nodiscard]] auto ThrusterOn() const -> bool;
  [[nodiscard]] auto TriggerDown() const -> bool;
  [[nodiscard]] auto ShieldStatus() const -> const health_status&;
  [[nodiscard]] auto ShieldsUp() const -> bool;
  [[nodiscard]] auto CanShoot() -> bool;
  [[nodiscard]] auto EmitThrustParticle() -> bool;
  [[nodiscard]] auto FireMode() const -> fire_mode;

private:

  auto UpdateWhenDestoyed(float interval, bool enableControl) -> void;
  auto UpdateWhenActive(float interval, bool enableControl) -> void;
  static [[nodiscard]] auto GetUpdatedAngle(D2D1_POINT_2F position, float direction, D2D1_POINT_2F destination, float interval) -> float;
  static [[nodiscard]] auto GetUpdatedPosition(D2D1_POINT_2F position, VELOCITY_2F velocity, float interval) -> D2D1_POINT_2F;
  static [[nodiscard]] auto SwitchFireMode(fire_mode fireMode) -> fire_mode;

private:

  static constexpr float m_thrustPower { 200.0f };

private:

  std::shared_ptr<player_state> m_state;
  
  float m_thrust { 0 };
  bool m_triggerDown { false };
  bool m_shieldsUp { false };

  reload_counter m_playerReloadCounter { 1.0f / 3.0f, 1 };
  fire_mode m_fireMode { fire_mode::one };

  reload_counter m_thrustEmmisionCounter { 1.0f / 10.0f, 1 };

  std::optional<D2D1_POINT_2F> m_destination;

};

inline [[nodiscard]] auto player_ship::Position() const -> D2D1_POINT_2F
{
  return m_state->m_position;
}

inline [[nodiscard]] auto player_ship::PreviousPosition() const -> D2D1_POINT_2F
{
  return m_state->m_previousPosition;
}

inline [[nodiscard]] auto player_ship::Angle() const -> float
{
  return m_state->m_angle;
}

inline [[nodiscard]] auto player_ship::Velocity() const -> VELOCITY_2F
{
  return m_state->m_velocity;
}

inline [[nodiscard]] auto player_ship::Destroyed() const -> bool
{
  return m_state->m_destroyed;
}

inline auto player_ship::SetAngle(float angle) -> void
{
  m_state->m_angle = angle;
}

inline auto player_ship::Rotate(float angle) -> void
{
  m_state->m_angle = direct2d::RotateAngle(m_state->m_angle, angle);
}

inline auto player_ship::SetThrust(float value) -> void
{
  m_thrust = value * m_thrustPower;
}

inline auto player_ship::ApplyDamage(int value) -> void
{
  if( !m_shieldsUp && m_state->m_shieldStatus.ApplyDamage(value) == 0 )
  {
    m_state->m_destroyed = true;
  }
}

inline auto player_ship::ApplyFatalDamage() -> void
{
  m_state->m_shieldStatus.ApplyFatalDamage();
  m_state->m_destroyed = true;
}

inline auto player_ship::Destroy() -> void
{
  m_state->m_destroyed = true;
}

inline [[nodiscard]] auto player_ship::ThrusterOn() const -> bool
{
  return m_state->m_thrusterOn;
}

inline [[nodiscard]] auto player_ship::TriggerDown() const -> bool
{
  return m_triggerDown;
}

inline [[nodiscard]] auto player_ship::ShieldStatus() const -> const health_status&
{
  return m_state->m_shieldStatus;
}

inline [[nodiscard]] auto player_ship::ShieldsUp() const -> bool
{
  return m_shieldsUp;
}

inline [[nodiscard]] auto player_ship::CanShoot() -> bool
{
  return m_triggerDown && !m_shieldsUp && m_playerReloadCounter.Get(1) == 1;
}

inline [[nodiscard]] auto player_ship::EmitThrustParticle() -> bool
{
  return m_state->m_thrusterOn && m_thrustEmmisionCounter.Get(1) == 1;
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
