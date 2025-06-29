#pragma once

#include "base_object.h"
#include "reload_counter.h"
#include "health_status.h"
#include "fractional_counter.h"
#include "object_velocity.h"

class player_ship_state : public base_object
{

public:

  struct thruster_state
  {
    float power;
    float angle;
  };

  player_ship_state(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) noexcept;

  auto Update(VELOCITY_2F environmentalForces, float airResistance, float interval, bool levelComplete) -> void;

  auto ApplyDamage(int value) -> void;
  auto ApplyFatalDamage() -> void;

  [[nodiscard]] auto ShieldStatus() const -> const health_status&;

  [[nodiscard]] auto CanShoot() -> bool;
  [[nodiscard]] auto ShootAngle() const noexcept -> std::optional<float>;
  [[nodiscard]] auto SetShootAngle(float value) noexcept -> void;
  [[nodiscard]] auto ResetShootAngle() noexcept -> void;

  auto SetThrusterOn(float value, float angle) -> void;
  auto SetThrusterOff() -> void;
  [[nodiscard]] auto BaseThrusterPower() const noexcept -> float;
  [[nodiscard]] auto ThrusterPower() const noexcept -> float;
  
  auto SetRotationSpeed(float value) -> void;
  auto RotateBy(float angle, float interval) -> void;

private:

  auto UpdateWhenActive(VELOCITY_2F environmentalForces, float airResistance, float interval) -> void;
  auto UpdateWhenCelebrating(float interval) -> void;

private:

  object_velocity m_velocity;
  fractional_counter m_stateChange;
  health_status m_shieldStatus { 10 };
  reload_counter m_playerReloadCounter { 1.0f / 10.0f, 1 };
  std::optional<thruster_state> m_thrusterState;
  float m_rotationSpeed;
  std::optional<float> m_shootAngle;

};

inline player_ship_state::player_ship_state(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) noexcept :
  base_object { position, scale, angle }, m_stateChange { 0.2f }, m_thrusterState { std::nullopt }, m_rotationSpeed { 0.0f }
{
}

inline auto player_ship_state::ApplyDamage(int value) -> void
{
  if( m_shieldStatus.ApplyDamage(value) == 0 )
  {
    Destroy();
  }
}

inline auto player_ship_state::ApplyFatalDamage() -> void
{
  m_shieldStatus.ApplyFatalDamage();
  Destroy();
}

inline [[nodiscard]] auto player_ship_state::ShieldStatus() const -> const health_status&
{
  return m_shieldStatus;
}

inline [[nodiscard]] auto player_ship_state::CanShoot() -> bool
{
  return !m_destroyed && m_playerReloadCounter.Get(1, true) == 1;
}

inline auto player_ship_state::ShootAngle() const noexcept -> std::optional<float>
{
  return m_shootAngle;
}

inline auto player_ship_state::SetShootAngle(float value) noexcept -> void
{
  m_shootAngle = value;
}

inline auto player_ship_state::ResetShootAngle() noexcept -> void
{
  m_shootAngle = std::nullopt;
}

inline auto player_ship_state::SetThrusterOn(float power, float angle) -> void
{
  m_thrusterState = { power, angle };
}

inline auto player_ship_state::SetThrusterOff() -> void
{
  m_thrusterState = std::nullopt;
}

inline auto player_ship_state::BaseThrusterPower() const noexcept -> float
{
  return m_thrusterState.has_value() ? m_thrusterState.value().power : 0.0f;
}

inline auto player_ship_state::ThrusterPower() const noexcept -> float
{
  constexpr float thrustPowerMultiplier = 1600.0f;
  return m_thrusterState.has_value() ? m_thrusterState.value().power * thrustPowerMultiplier : 0.0f;
}

inline auto player_ship_state::SetRotationSpeed(float value) -> void
{
  constexpr float rotationSpeedMultiplier = 100.0f;
  m_rotationSpeed = value * rotationSpeedMultiplier;
}

inline auto player_ship_state::RotateBy(float angle, float interval) -> void
{
  m_angle += angle * interval;
}
