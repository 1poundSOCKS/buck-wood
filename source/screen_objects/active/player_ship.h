#ifndef _player_ship_
#define _player_ship_

#include "play_event.h"
#include "object_input_data.h"
#include "geometry.h"
#include "path_geometry.h"
#include "transformed_path_geometry.h"
#include "reload_timer.h"
#include "moving_body.h"

class player_ship
{
public:

  enum class state { alive, dead };
  using points_collection = std::vector<game_point>;

  player_ship();
  player_ship(const game_point& position);

  auto SetAngle(float angle) -> void;
  auto SetThrusterOn(bool on) -> void;
  auto Destroy() -> void;

  [[nodiscard]] auto Position() const -> const game_point&;
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Velocity() const -> const game_velocity&;
  [[nodiscard]] auto State() const -> state;
  [[nodiscard]] auto ThrusterOn() const -> bool;
  [[nodiscard]] auto Destroyed() const -> bool;

  [[nodiscard]] auto Geometry() const -> const transformed_path_geometry&;
  auto GetTransformedThrusterGeometry(std::back_insert_iterator<points_collection> pointsInserter) const -> void;
  [[nodiscard]] auto CanShoot() -> bool;

  auto Update(int64_t tickCount) -> void;

private:

  inline static const auto m_playerThrust { 400.0f };

  void UpdateShipGeometryData();

  static auto GetShotTimeInterval() -> int64_t;
  static constexpr auto GetShotTimeNumerator() -> int64_t;
  static constexpr auto GetShotTimeDenominator() -> int64_t;

  static inline int m_shotTimeNumerator = 1;
  static inline int m_shotTimeDenominator = 20;

  state m_state { state::alive };
  moving_body m_movingBody;
  float m_angle = 0;
  bool m_thrusterOn = false;
  bool m_destroyed { false };

  reload_timer m_reloadTimer;
  bool m_reloaded { false };

  path_geometry m_geometry;
  transformed_path_geometry m_transformedGeometry;
};

inline auto player_ship::SetAngle(float angle) -> void
{
  m_angle = angle;
}

inline auto player_ship::SetThrusterOn(bool on) -> void
{
  m_thrusterOn = on;
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
  return m_thrusterOn;
}

[[nodiscard]] inline auto player_ship::Destroyed() const -> bool
{
  return m_destroyed;
}

#endif
