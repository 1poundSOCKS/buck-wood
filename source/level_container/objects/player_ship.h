#pragma once

#include "framework.h"
#include "base_object.h"
#include "play_event.h"
#include "reload_timer.h"
#include "reload_counter.h"
#include "health_status.h"
#include "level_object_movement.h"
#include "player_ship_state.h"

class player_ship
{

public:

  player_ship(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity);

  auto Update(float interval) -> void;
  auto UpdateVelocity(VELOCITY_2F changeInVelocity, float interval) -> void;
  auto UpdateAngle() -> void;
  auto UpdatePosition(float interval) -> POINT_2F;

  [[nodiscard]] auto Position() const noexcept -> POINT_2F;
  [[nodiscard]] auto Scale() const noexcept -> SCALE_2F;
  [[nodiscard]] auto Angle() const noexcept -> float;
  [[nodiscard]] auto Age() const noexcept -> float;
  [[nodiscard]] auto Destroyed() const noexcept -> bool;
  auto Destroy() noexcept -> void;

  [[nodiscard]] auto State() const -> std::shared_ptr<player_ship_state>;

  auto Rotate(float angle) -> void;
  auto ApplyDamage(int value) -> void;
  auto ApplyFatalDamage() -> void;

  [[nodiscard]] auto Velocity() const noexcept -> VELOCITY_2F;
  [[nodiscard]] auto ShieldStatus() const -> const health_status&;
  [[nodiscard]] auto CanShoot() -> bool;

  static [[nodiscard]] auto CalculateVelocity(float controlX, float controlY) -> VELOCITY_2F;

private:

  auto UpdateWhenActive(float interval) -> void;
  auto UpdateWhenCelebrating(float interval) -> void;

private:

  static constexpr float m_thrustPower { 3000.0f };
  static constexpr float m_friction { 0.05f };
  static constexpr SIZE_F m_objectSize { 60, 60 };

private:

  std::shared_ptr<player_ship_state> m_state;
  health_status m_shieldStatus { 10 };
  reload_counter m_playerReloadCounter { 1.0f / 3.0f, 1 };

};

inline auto player_ship::Rotate(float angle) -> void
{
  m_state->SetAngle(direct2d::RotateAngle(m_state->Angle(), angle));
}

inline auto player_ship::ApplyDamage(int value) -> void
{
  if( m_shieldStatus.ApplyDamage(value) == 0 )
  {
    m_state->Destroy();
  }
}

inline auto player_ship::ApplyFatalDamage() -> void
{
  m_shieldStatus.ApplyFatalDamage();
  m_state->Destroy();
}

inline [[nodiscard]] auto player_ship::Velocity() const noexcept -> VELOCITY_2F
{
  return m_state->Velocity();
}

inline [[nodiscard]] auto player_ship::ShieldStatus() const -> const health_status&
{
  return m_shieldStatus;
}

inline [[nodiscard]] auto player_ship::CanShoot() -> bool
{
  return !m_state->Destroyed() && m_playerReloadCounter.Get(1, true) == 1;
}

inline auto player_ship::CalculateVelocity(float controlX, float controlY) -> VELOCITY_2F
{
  constexpr float thrustPower { 3000.0f };
  return { controlX * thrustPower, controlY * thrustPower };
}
