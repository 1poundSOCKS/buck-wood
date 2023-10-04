#pragma once

#include "play_event.h"
#include "geometry.h"
#include "path_geometry.h"
#include "transformed_path_geometry.h"
#include "reload_timer.h"
#include "directional_body.h"
#include "health_status.h"

class player_ship
{
public:

  enum class state { alive, dead };

  using points_collection = std::vector<game_point>;
  using shield_status = std::shared_ptr<health_status>;

  player_ship();
  player_ship(const game_point& position);

  auto SetAngle(float angle) -> void;
  auto Rotate(float angle) -> void;
  auto SetThrust(float value) -> void;
  auto Update(float interval) -> void;
  auto ApplyDamage(int value) -> void;
  auto ApplyFatalDamage() -> void;
  auto Destroy() -> void;

  [[nodiscard]] auto Position() const -> const game_point&;
  [[nodiscard]] auto PreviousPosition() const -> const game_point&;
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Velocity() const -> const game_velocity&;
  [[nodiscard]] auto State() const -> state;
  [[nodiscard]] auto ThrusterOn() const -> bool;
  [[nodiscard]] auto ShieldStatus() const -> const shield_status&;
  [[nodiscard]] auto Destroyed() const -> bool;

  [[nodiscard]] auto Geometry() const -> const transformed_path_geometry&;
  auto GetTransformedThrusterGeometry(std::back_insert_iterator<points_collection> pointsInserter) const -> void;
  [[nodiscard]] auto CanShoot() -> bool;

private:

  inline static const auto m_thrustPower { 400.0f };

  void UpdateShipGeometryData();

  static auto GetShotTimeInterval() -> int64_t;

  state m_state { state::alive };
  directional_body m_body;
  directional_body m_previousState;
  float m_thrust = 0;
  shield_status m_shieldStatus { std::make_shared<health_status>(10) };
  bool m_destroyed { false };

  path_geometry m_geometry;
  transformed_path_geometry m_transformedGeometry;
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

[[nodiscard]] inline auto player_ship::Position() const -> const game_point&
{
  return m_body.Position();
}

[[nodiscard]] inline auto player_ship::PreviousPosition() const -> const game_point&
{
  return m_previousState.Position();
}

[[nodiscard]] inline auto player_ship::Angle() const -> float
{
  return m_body.Angle();
}

[[nodiscard]] inline auto player_ship::Velocity() const -> const game_velocity&
{
  return m_body.Velocity();
}

[[nodiscard]] inline auto player_ship::State() const -> state
{
  return m_state;
}

[[nodiscard]] inline auto player_ship::ThrusterOn() const -> bool
{
  return m_thrust > 0 ? true : false;
}

[[nodiscard]] inline auto player_ship::ShieldStatus() const -> const shield_status&
{
  return m_shieldStatus;
}

[[nodiscard]] inline auto player_ship::Destroyed() const -> bool
{
  return m_destroyed;
}
