#ifndef _player_ship_
#define _player_ship_

#include "play_event.h"
#include "collision_data.h"
#include "collision_effect.h"
#include "object_input_data.h"
#include "path_geometry.h"
#include "transformed_path_geometry.h"

class player_ship
{
public:

  enum class state { alive, dead };
  using points_collection = std::vector<game_point>;

  player_ship();
  player_ship(float x, float y);

  auto SetAngle(float angle) -> void;
  auto SetThrusterOn(bool on) -> void;

  [[nodiscard]] auto Position() const -> game_point;
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto State() const -> state;
  [[nodiscard]] auto ThrusterOn() const -> bool;

  [[nodiscard]] auto Points() const -> const points_collection&;
  auto GetTransformedThrusterGeometry(std::back_insert_iterator<points_collection> pointsInserter) const -> void;
  [[nodiscard]] auto Geometry() const -> const transformed_path_geometry&;
  [[nodiscard]] auto CanShoot() -> bool;

  auto Update(int64_t tickCount) -> void;

private:

  using lines_collection = std::vector<game_line>;

  void UpdateShipGeometryData();
  auto GetTransformedShipPointsGeometry(std::back_insert_iterator<points_collection> linesInserter) const -> void;

  static game_closed_object GetPlayerObject();

  static constexpr std::array<game_point, 3> GetPlayerGeometryData();
  static constexpr std::array<game_point, 2> GetPlayerThrusterGeometryData();

  static auto GetShotTimeInterval() -> int64_t;
  static constexpr auto GetShotTimeNumerator() -> int64_t;
  static constexpr auto GetShotTimeDenominator() -> int64_t;

  static inline int m_shotTimeNumerator = 1;
  static inline int m_shotTimeDenominator = 20;

  int64_t m_shotTimerInterval = 0;
  int64_t m_shotTimer = 0;

  state m_state = state::alive;  
  float m_x = 0;
  float m_y = 0;
  float m_angle = 0;
  bool m_thrusterOn = false;
  float m_velocityX = 0;
  float m_velocityY = 0;

  points_collection m_points;
  lines_collection m_lines;

  path_geometry m_geometry;
  transformed_path_geometry m_transformedGeometry;
};

#endif