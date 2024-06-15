#pragma once

#include "framework.h"
#include "base_object.h"
#include "object_velocity.h"
#include "play_event.h"
#include "reload_timer.h"
#include "reload_counter.h"
#include "health_status.h"
#include "level_cell_movement.h"

class player_ship
{

public:

  player_ship(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity);

  auto Update(float interval) -> void;

  [[nodiscard]] auto Position() const noexcept -> POINT_2F;
  [[nodiscard]] auto Scale() const noexcept -> SCALE_2F;
  [[nodiscard]] auto Angle() const noexcept -> float;
  [[nodiscard]] auto Age() const noexcept -> float;
  [[nodiscard]] auto Destroyed() const noexcept -> bool;
  auto Destroy() noexcept -> void;

  auto Rotate(float angle) -> void;
  auto ApplyDamage(int value) -> void;
  auto ApplyFatalDamage() -> void;
  auto Set(std::shared_ptr<level_cell_movement> value) -> void;

  [[nodiscard]] auto State() const noexcept -> std::shared_ptr<base_object>;
  [[nodiscard]] auto Velocity() const noexcept -> VELOCITY_2F;
  [[nodiscard]] auto ThrusterOn() const -> bool;
  [[nodiscard]] auto TriggerDown() const -> bool;
  [[nodiscard]] auto ShieldStatus() const -> const health_status&;
  [[nodiscard]] auto CanShoot() -> bool;
  [[nodiscard]] auto EmitThrustParticle() -> bool;
  [[nodiscard]] auto ThrustParticleDirection() const -> float;
  [[nodiscard]] auto ShootAngle() const noexcept -> float;

private:

  auto UpdateWhenActive(float interval) -> void;
  auto UpdateWhenCelebrating(float interval) -> void;

private:

  static constexpr float m_thrustPower { 3000.0f };
  static constexpr float m_friction { 0.05f };
  static constexpr SIZE_F m_objectSize { 60, 60 };

private:

  std::shared_ptr<base_object> m_state;
  object_velocity m_velocity;
  health_status m_shieldStatus { 10 };
  bool m_triggerDown { false };
  reload_counter m_playerReloadCounter { 1.0f / 3.0f, 1 };
  reload_counter m_thrustEmmisionCounter { 1.0f / 10.0f, 2 };
  float m_shootAngle;
  std::optional<POINT_2F> m_leftThumbstickPosition;
  std::shared_ptr<level_cell_movement> m_levelCellMovement;

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

inline auto player_ship::Set(std::shared_ptr<level_cell_movement> value) -> void
{
  m_levelCellMovement = value;
}

inline auto player_ship::State() const noexcept -> std::shared_ptr<base_object>
{
  return m_state;
}

inline [[nodiscard]] auto player_ship::Velocity() const noexcept -> VELOCITY_2F
{
  return m_velocity.Get();
}

inline [[nodiscard]] auto player_ship::ThrusterOn() const -> bool
{
  return !m_state->Destroyed() && m_leftThumbstickPosition;
}

inline [[nodiscard]] auto player_ship::TriggerDown() const -> bool
{
  return !m_state->Destroyed() && m_triggerDown;
}

inline [[nodiscard]] auto player_ship::ShieldStatus() const -> const health_status&
{
  return m_shieldStatus;
}

inline [[nodiscard]] auto player_ship::CanShoot() -> bool
{
  return !m_state->Destroyed() && TriggerDown() && m_playerReloadCounter.Get(1, true) == 1;
}

inline [[nodiscard]] auto player_ship::ThrustParticleDirection() const -> float
{
  auto angle = m_leftThumbstickPosition ? direct2d::CalculateDirection({m_leftThumbstickPosition->x, m_leftThumbstickPosition->y}) : 0;
  return direct2d::RotateAngle(angle, 180);
}

inline [[nodiscard]] auto player_ship::EmitThrustParticle() -> bool
{
  return !m_state->Destroyed() && m_leftThumbstickPosition && m_thrustEmmisionCounter.Get(1, true) == 1;
}

inline auto player_ship::ShootAngle() const noexcept -> float
{
  return m_shootAngle;
}
