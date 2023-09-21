#ifndef _player_ship_
#define _player_ship_

#include "play_event.h"
#include "object_input_data.h"
#include "geometry.h"
#include "path_geometry.h"
#include "transformed_path_geometry.h"
#include "reload_timer.h"
#include "moving_body.h"
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
  auto SetDirection(float direction) -> void;
  auto Rotate(float angle) -> void;
  auto SetThrust(float value) -> void;
  auto Update(float interval) -> void;
  auto ApplyDamage(int value) -> void;
  auto ApplyFatalDamage() -> void;
  auto Destroy() -> void;

  [[nodiscard]] auto Position() const -> const game_point&;
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

  inline static const auto m_playerThrust { 20.0f };

  void UpdateShipGeometryData();

  static auto GetShotTimeInterval() -> int64_t;

  state m_state { state::alive };
  moving_body m_movingBody;
  float m_angle = 0;
  float m_thrust = 0;
  shield_status m_shieldStatus { std::make_shared<health_status>(10) };
  bool m_destroyed { false };
  bool m_autoDecelerate { false };

  path_geometry m_geometry;
  transformed_path_geometry m_transformedGeometry;
};

inline auto player_ship::SetAngle(float angle) -> void
{
  m_angle = angle;
}

inline auto player_ship::SetDirection(float direction) -> void
{
  m_movingBody.SetDirection(direction);
}

inline auto player_ship::Rotate(float angle) -> void
{
  m_angle += angle;
  m_angle += 360.0f;
  
  while( m_angle >= 360.0f )
  {
    m_angle -= 360.0f;
  }
}

inline auto player_ship::SetThrust(float value) -> void
{
  m_thrust = value * m_playerThrust;
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
  return m_movingBody.Position();
}

[[nodiscard]] inline auto player_ship::Angle() const -> float
{
  return m_angle;
}

[[nodiscard]] inline auto player_ship::Velocity() const -> const game_velocity&
{
  return m_movingBody.Velocity();
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

#endif
