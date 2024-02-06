#pragma once

#include "framework.h"
#include "play_event.h"
#include "reload_timer.h"
#include "health_status.h"

class player_ship
{

public:

  enum class state { alive, dead };

  using points_collection = std::vector<D2D1_POINT_2F>;
  using shield_status = std::shared_ptr<health_status>;

public:

  player_ship(D2D1_POINT_2F position);

  [[nodiscard]] auto Scale() const -> SCALE_2F { return { 1.0f, 1.0f }; };
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> D2D1_POINT_2F;
  [[nodiscard]] auto Destroyed() const -> bool;
  auto Update(float interval) -> void;

  auto SetAngle(float angle) -> void;
  auto Rotate(float angle) -> void;
  auto SetThrust(float value) -> void;
  auto ApplyDamage(int value) -> void;
  auto ApplyFatalDamage() -> void;
  auto Destroy() -> void;
  auto SetDestination(std::optional<D2D1_POINT_2F> value) -> void;

  [[nodiscard]] auto PreviousPosition() const -> D2D1_POINT_2F;
  [[nodiscard]] auto Velocity() const -> VELOCITY_2F;
  [[nodiscard]] auto State() const -> state;
  [[nodiscard]] auto ThrusterOn() const -> bool;
  [[nodiscard]] auto TriggerDown() const -> bool;
  [[nodiscard]] auto ShieldStatus() const -> const shield_status&;
  [[nodiscard]] auto ShieldsUp() const -> bool;
  // [[nodiscard]] auto CanShoot() const -> bool;

private:

  static [[nodiscard]] auto GetUpdatedAngle(D2D1_POINT_2F position, float direction, D2D1_POINT_2F destination) -> float;
  static [[nodiscard]] auto GetUpdatedPosition(D2D1_POINT_2F position, VELOCITY_2F velocity, float interval) -> D2D1_POINT_2F;

private:

  static constexpr float m_thrustPower { 400.0f };
  state m_state { state::alive };
  D2D1_POINT_2F m_position { 0, 0 };
  D2D1_POINT_2F m_previousPosition { 0, 0 };
  float m_angle { 0 };
  VELOCITY_2F m_velocity { 0, 0 };
  float m_thrust { 0 };
  bool m_thrusterOn { false };
  bool m_triggerDown { false };
  bool m_shieldsUp { false };
  shield_status m_shieldStatus { std::make_shared<health_status>(10) };
  bool m_destroyed { false };
  std::optional<D2D1_POINT_2F> m_destination;

};

inline [[nodiscard]] auto player_ship::Position() const -> D2D1_POINT_2F
{
  return m_position;
}

inline [[nodiscard]] auto player_ship::PreviousPosition() const -> D2D1_POINT_2F
{
  return m_previousPosition;
}

inline [[nodiscard]] auto player_ship::Angle() const -> float
{
  return m_angle;
}

inline [[nodiscard]] auto player_ship::Velocity() const -> VELOCITY_2F
{
  return m_velocity;
}

inline [[nodiscard]] auto player_ship::Destroyed() const -> bool
{
  return m_destroyed;
}

inline auto player_ship::SetAngle(float angle) -> void
{
  m_angle = angle;
}

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
  if( m_shieldStatus->ApplyDamage(value) == 0 )
  {
    Destroy();
  }
}

inline auto player_ship::ApplyFatalDamage() -> void
{
  m_shieldStatus->ApplyFatalDamage();
  Destroy();
}

inline auto player_ship::Destroy() -> void
{
  m_destroyed = true;
}

inline auto player_ship::SetDestination(std::optional<D2D1_POINT_2F> value) -> void
{
  m_destination = value;
}

inline [[nodiscard]] auto player_ship::State() const -> state
{
  return m_state;
}

inline [[nodiscard]] auto player_ship::ThrusterOn() const -> bool
{
  return m_thrusterOn;
}

inline [[nodiscard]] auto player_ship::TriggerDown() const -> bool
{
  return m_triggerDown;
}

inline [[nodiscard]] auto player_ship::ShieldStatus() const -> const shield_status&
{
  return m_shieldStatus;
}

inline [[nodiscard]] auto player_ship::ShieldsUp() const -> bool
{
  return m_shieldsUp;
}
