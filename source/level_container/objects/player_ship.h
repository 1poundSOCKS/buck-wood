#pragma once

#include "framework.h"
#include "play_event.h"
#include "reload_timer.h"
#include "directional_body.h"
#include "health_status.h"
#include "game_scale.h"

class player_ship
{

public:

  enum class state { alive, dead };

  using points_collection = std::vector<D2D1_POINT_2F>;
  using shield_status = std::shared_ptr<health_status>;

public:

  player_ship(D2D1_POINT_2F position);

  [[nodiscard]] auto Scale() const -> game_scale { return game_scale { 1.0f, 1.0f }; };
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> D2D1_POINT_2F;
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
  [[nodiscard]] auto ShieldStatus() const -> const shield_status&;
  [[nodiscard]] auto Destroyed() const -> bool;
  [[nodiscard]] auto CanShoot() -> bool;
  [[nodiscard]] auto RelativePosition(float angle, float cx, float cy) const -> D2D1_POINT_2F;
  [[nodiscard]] auto RelativeVelocity(float angle, float speed) const -> VELOCITY_2F;

private:

  static [[nodiscard]] auto GetUpdatedAngle(D2D1_POINT_2F position, float direction, D2D1_POINT_2F destination) -> float;
  static [[nodiscard]] auto GetUpdatedPosition(D2D1_POINT_2F position, VELOCITY_2F velocity, float direction, float interval) -> D2D1_POINT_2F;

private:

  static constexpr float m_thrustPower { 400.0f };
  state m_state { state::alive };
  directional_body m_body;
  directional_body m_previousState;
  float m_thrust { 0 };
  bool m_thrusterOn { false };
  shield_status m_shieldStatus { std::make_shared<health_status>(10) };
  bool m_destroyed { false };
  std::optional<D2D1_POINT_2F> m_destination;

};

inline auto player_ship::SetAngle(float angle) -> void
{
  m_body.SetAngle(angle);
}

inline auto player_ship::Rotate(float angle) -> void
{
  m_body.Rotate(angle);
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

inline [[nodiscard]] auto player_ship::Position() const -> D2D1_POINT_2F
{
  return m_body.Position();
}

inline [[nodiscard]] auto player_ship::PreviousPosition() const -> D2D1_POINT_2F
{
  return m_previousState.Position();
}

inline [[nodiscard]] auto player_ship::Angle() const -> float
{
  return m_body.Angle();
}

inline [[nodiscard]] auto player_ship::Velocity() const -> VELOCITY_2F
{
  return m_body.Velocity();
}

inline [[nodiscard]] auto player_ship::State() const -> state
{
  return m_state;
}

inline [[nodiscard]] auto player_ship::ThrusterOn() const -> bool
{
  return m_thrusterOn;
}

inline [[nodiscard]] auto player_ship::ShieldStatus() const -> const shield_status&
{
  return m_shieldStatus;
}

inline [[nodiscard]] auto player_ship::Destroyed() const -> bool
{
  return m_destroyed;
}

inline [[nodiscard]] auto player_ship::RelativePosition(float angle, float cx, float cy) const -> D2D1_POINT_2F
{
  return m_body.RelativePosition(angle, cx, cy);
}

inline [[nodiscard]] auto player_ship::RelativeVelocity(float angle, float speed) const -> VELOCITY_2F
{
  return direct2d::CombineVelocities(m_body.Velocity(), direct2d::CalculateVelocity(speed, angle));
}
