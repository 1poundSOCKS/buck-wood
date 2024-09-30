#pragma once

#include "base_object.h"
#include "reload_counter.h"
#include "health_status.h"
#include "fractional_counter.h"

class player_ship_state : public base_object
{

public:

  player_ship_state(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) noexcept;

  auto Update(float interval) -> void;

  auto ApplyDamage(int value) -> void;
  auto ApplyFatalDamage() -> void;

  [[nodiscard]] auto ShieldStatus() const -> const health_status&;

  [[nodiscard]] auto CanShoot() -> bool;
  [[nodiscard]] auto ShootAngle() const noexcept -> std::optional<float>;
  [[nodiscard]] auto SetShootAngle(float value) noexcept -> void;
  [[nodiscard]] auto ResetShootAngle() noexcept -> void;

private:

  auto UpdateWhenActive(float interval) -> void;
  auto UpdateWhenCelebrating(float interval) -> void;

private:

  fractional_counter m_stateChange;
  health_status m_shieldStatus { 10 };
  reload_counter m_playerReloadCounter { 1.0f / 10.0f, 1 };
  std::optional<float> m_shootAngle;

};

inline player_ship_state::player_ship_state(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) noexcept :
  base_object { position, scale, angle }, m_stateChange { 0.2f }
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
  m_angle = value;
}

inline auto player_ship_state::ResetShootAngle() noexcept -> void
{
  m_shootAngle = std::nullopt;
}
