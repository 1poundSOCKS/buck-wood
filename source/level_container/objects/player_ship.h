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

class player_ship : public base_object, public object_velocity
{

public:

  using points_collection = std::vector<D2D1_POINT_2F>;
  enum class fire_mode { none, one, two };

public:

  player_ship(POINT_2F position);

  auto Update(float interval, const level_cell_collection& cells) -> void;

  auto Rotate(float angle) -> void;
  // auto SetThrust(float value) -> void;
  auto ApplyDamage(int value) -> void;
  auto ApplyFatalDamage() -> void;

  // [[nodiscard]] auto ThrusterOn() const -> bool;
  [[nodiscard]] auto TriggerDown() const -> bool;
  [[nodiscard]] auto ShieldStatus() const -> const health_status&;
  // [[nodiscard]] auto ShieldsUp() const -> bool;
  [[nodiscard]] auto CanShoot() -> bool;
  // [[nodiscard]] auto CanFireMissile() -> bool;
  // [[nodiscard]] auto EmitThrustParticle() -> bool;
  // [[nodiscard]] auto FireMode() const -> fire_mode;

  // [[nodiscard]] auto TargettingActive() const -> bool;
  [[nodiscard]] auto ShootAngle() const noexcept -> float;

private:

  auto UpdateWhenActive(float interval, const level_cell_collection& cells) -> void;
  auto UpdateWhenCelebrating(float interval) -> void;
  auto UpdatePosition(float interval, const level_cell_collection& cells, POINT_2F movementControl) -> void;

  static [[nodiscard]] auto GetUpdatedAngle(D2D1_POINT_2F position, float direction, D2D1_POINT_2F destination, float interval) -> float;
  static [[nodiscard]] auto GetUpdatedPosition(D2D1_POINT_2F position, VELOCITY_2F velocity, float interval) -> D2D1_POINT_2F;
  static [[nodiscard]] auto SwitchFireMode(fire_mode fireMode) -> fire_mode;

  enum class control_direction_type { none, up, down, left, right };
  [[nodiscard]] auto DirectionalControlPressed() const -> control_direction_type;
  auto Visit(adjacent_cell_visitor& visitor, const level_cell_collection& cellCollection) -> void;

  static [[nodiscard]] auto ExpandRect(RECT_F rect, SIZE_F size) -> RECT_F;

private:

  static constexpr float m_thrustPower { 500.0f };

private:

  enum class movement_state { normal, dash };

  movement_state m_movementState { movement_state::normal };

  // bool m_thrusterOn { false };
  health_status m_shieldStatus { 10 };
  
  // float m_thrust { 0 };
  bool m_triggerDown { false };
  // bool m_shieldsUp { false };

  reload_counter m_playerReloadCounter { 1.0f / 3.0f, 1 };
  // fire_mode m_fireMode { fire_mode::one };

  reload_counter m_thrustEmmisionCounter { 1.0f / 10.0f, 2 };

  reload_counter m_dashTimer { 1.0f / 5.0f, 1 };

  std::optional<POINT_2F> m_destination;
  float m_shootAngle;

  VELOCITY_2F m_dashVelocity { 0, 0 };

};

inline auto player_ship::Rotate(float angle) -> void
{
  m_angle = direct2d::RotateAngle(m_angle, angle);
}

// inline auto player_ship::SetThrust(float value) -> void
// {
//   m_thrust = value * m_thrustPower;
// }

inline auto player_ship::ApplyDamage(int value) -> void
{
  // if( !m_shieldsUp && m_shieldStatus.ApplyDamage(value) == 0 )
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

// inline [[nodiscard]] auto player_ship::ThrusterOn() const -> bool
// {
//   return !m_destroyed && m_thrusterOn;
// }

inline [[nodiscard]] auto player_ship::TriggerDown() const -> bool
{
  return !m_destroyed && m_triggerDown;
}

inline [[nodiscard]] auto player_ship::ShieldStatus() const -> const health_status&
{
  return m_shieldStatus;
}

// inline [[nodiscard]] auto player_ship::ShieldsUp() const -> bool
// {
//   return !m_destroyed && m_shieldsUp;
// }

inline [[nodiscard]] auto player_ship::CanShoot() -> bool
{
  // return !m_destroyed && m_fireMode == fire_mode::one && TriggerDown() && !m_shieldsUp && m_playerReloadCounter.Get(1, true) == 1;
  return !m_destroyed && TriggerDown() && m_playerReloadCounter.Get(1, true) == 1;
}

// inline [[nodiscard]] auto player_ship::CanFireMissile() -> bool
// {
//   return !m_destroyed && m_fireMode == fire_mode::two && TriggerDown() && !m_shieldsUp && m_playerReloadCounter.Get(1, true) == 1;
// }

// inline [[nodiscard]] auto player_ship::EmitThrustParticle() -> bool
// {
//   return !m_destroyed && m_thrusterOn && m_thrustEmmisionCounter.Get(1, true) == 1;
// }

// inline [[nodiscard]] auto player_ship::FireMode() const -> fire_mode
// {
//   return m_fireMode;
// }

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

// inline [[nodiscard]] auto player_ship::TargettingActive() const -> bool
// {
//   return !Destroyed() && m_fireMode == fire_mode::two;
// }

inline auto player_ship::ShootAngle() const noexcept -> float
{
  return m_shootAngle;
}
