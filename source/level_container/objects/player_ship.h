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

  struct update_events
  {
    bool shot { false };
  };

  using points_collection = std::vector<game_point>;
  using shield_status = std::shared_ptr<health_status>;

public:

  player_ship() = default;
  player_ship(const game_point& position);

  auto SetAngle(float angle) -> void;
  auto Rotate(float angle) -> void;
  auto SetThrust(float value) -> void;
  auto ApplyDamage(int value) -> void;
  auto ApplyFatalDamage() -> void;
  auto Destroy() -> void;

  auto Update(float interval, float thrust, std::optional<float> angle, std::optional<float> rotation, bool shoot, update_events* updateEvents) -> void;

  [[nodiscard]] auto Scale() const -> game_scale { return game_scale { 1.0f, 1.0f }; };
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> const game_point&;

  [[nodiscard]] auto PreviousPosition() const -> const game_point&;
  [[nodiscard]] auto Velocity() const -> const game_velocity&;
  [[nodiscard]] auto State() const -> state;
  [[nodiscard]] auto ThrusterOn() const -> bool;
  [[nodiscard]] auto ShieldStatus() const -> const shield_status&;
  [[nodiscard]] auto Destroyed() const -> bool;
  [[nodiscard]] auto Geometry() const -> const transformed_path_geometry&;
  [[nodiscard]] auto CanShoot() -> bool;
  [[nodiscard]] auto RelativePosition(float angle, float cx, float cy) const -> game_point;
  [[nodiscard]] auto RelativeVelocity(float angle, float value) const -> game_velocity;

private:

  inline static const auto m_thrustPower { 400.0f };
  state m_state { state::alive };
  directional_body m_body;
  directional_body m_previousState;
  float m_thrust { 0 };
  reload_timer m_reloadTimer { 1.0f / 20.0f };
  shield_status m_shieldStatus { std::make_shared<health_status>(10) };
  bool m_destroyed { false };

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

inline [[nodiscard]] auto player_ship::Position() const -> const game_point&
{
  return m_body.Position();
}

inline [[nodiscard]] auto player_ship::PreviousPosition() const -> const game_point&
{
  return m_previousState.Position();
}

inline [[nodiscard]] auto player_ship::Angle() const -> float
{
  return m_body.Angle();
}

inline [[nodiscard]] auto player_ship::Velocity() const -> const game_velocity&
{
  return m_body.Velocity();
}

inline [[nodiscard]] auto player_ship::State() const -> state
{
  return m_state;
}

inline [[nodiscard]] auto player_ship::ThrusterOn() const -> bool
{
  return m_thrust > 0 ? true : false;
}

inline [[nodiscard]] auto player_ship::ShieldStatus() const -> const shield_status&
{
  return m_shieldStatus;
}

inline [[nodiscard]] auto player_ship::Destroyed() const -> bool
{
  return m_destroyed;
}

inline [[nodiscard]] auto player_ship::RelativePosition(float angle, float cx, float cy) const -> game_point
{
  return m_body.RelativePosition(angle, cx, cy);
}

inline [[nodiscard]] auto player_ship::RelativeVelocity(float angle, float value) const -> game_velocity
{
  return m_body.Velocity() + game_velocity { game_angle { angle }, value };
}
