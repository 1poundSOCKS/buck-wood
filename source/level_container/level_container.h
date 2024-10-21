#pragma once

#include "framework.h"
#include "default_object_collection.h"
#include "particle_collection.h"
#include "level_explosion.h"
#include "play_events.h"
#include "game_score.h"
#include "game_settings.h"
#include "collisions/particle_collision.h"
#include "default_object_geometry.h"
#include "default_object_geometry_collection.h"
#include "range_comparision_runner.h"
#include "geometry_collision.h"
#include "geometry_containment.h"
#include "level_collision_geometry.h"
#include "geometry_collision_runner.h"
#include "level_object_movement.h"
#include "player_controls.h"

class level_container
{

public:

  enum class object_type { portal_entry, portal_exit, player, enemy_stalker, enemy_random, enemy_turret, power_up, cell, boundary_walls };

  level_container();
  level_container(range_comparison_runner::execution ex, collision_type collisionType);
  level_container(const level_container& levelContainer) = delete;

  auto AddBoundaryWalls(int levelIndex, std::ranges::input_range auto&& pointData) -> void;

  auto InitializePlayer() -> void;

  auto ObjectCount(object_type objectType) const noexcept -> std::size_t;

  [[nodiscard]] auto UnoccupiedFloorCellCount() const noexcept -> size_t;
  [[nodiscard]] auto UnoccupiedFloorCell(size_t index) const noexcept -> cell_id;

  auto Update(float interval, D2D1_RECT_F viewRect) -> std::optional<player_ship_state>;
  auto UpdateVelocity(VELOCITY_2F changeInVelocity, float interval) -> void;

  [[nodiscard]] auto PlayerState() const noexcept -> const player_ship_state&;

  [[nodiscard]] auto Boundary() const -> RECT_F;

  [[nodiscard]] auto CentrePoint() const noexcept -> POINT_2F;

  [[nodiscard]] auto Objects() const noexcept -> const default_object_collection&;
  [[nodiscard]] auto Objects() noexcept -> default_object_collection&;

  [[nodiscard]] auto Particles() const noexcept -> const particle_collection&;
  [[nodiscard]] auto Particles() noexcept -> particle_collection&;

private:

  auto UpdateObject(player_ship& object, float interval) -> void;
  auto UpdateObject(enemy_ship& object, float interval) -> void;
  auto UpdateObject(auto& object, float interval) -> void;

  auto VisitObject(player_ship& object) -> void;
  auto VisitObject(enemy_ship& object) -> void;
  auto VisitObject(auto &object) -> void;

  auto DoCollisions() -> void;

  auto OnCollision(player_bullet& bullet, enemy_ship& enemy, geometry_collision::result result) -> void;
  auto OnCollision(player_bullet& bullet, level_cell& wall, geometry_collision::result result) -> void;
  auto OnCollision(enemy_bullet& bullet, level_cell& wall, geometry_collision::result result) -> void;
  auto OnCollision(player_ship& playerShip, enemy_ship& enemy, geometry_collision::result result) -> void;
  auto OnCollision(player_ship& playerShip, enemy_bullet& enemyBullet, geometry_collision::result result) -> void;
  auto OnCollision(player_ship& playerShip, power_up& powerUp, geometry_collision::result result) -> void;
  auto OnCollision(auto&& object, boundary_walls& boundaryWalls, geometry_collision::result result) -> void;
  auto OnCollision(auto&& object1, auto&& object2, geometry_collision::result result) -> void;

private:

  static constexpr int m_cellSize { 250 };
  static constexpr auto m_cellWidth { static_cast<float>(m_cellSize) };
  static constexpr auto m_cellHeight { static_cast<float>(m_cellSize) };

  std::shared_ptr<level_cell_collection> m_cells;
  std::shared_ptr<level_object_movement> m_objectMovement;

  RECT_F m_boundary;

  std::shared_ptr<player_ship_state> m_playerState;
  std::optional<player_ship_state> m_player;

  default_object_collection m_objects;

  level_collision_geometry m_collisionGeometry;
  geometry_collision_runner m_collisionRunner;

  particle_collection m_particles;
  particle_collision m_particleCollisionRunner;

  size_t m_powerUpCount { 0 };
};

inline auto level_container::PlayerState() const noexcept -> const player_ship_state &
{
  return *m_playerState;
}

auto level_container::AddBoundaryWalls(int levelIndex, std::ranges::input_range auto&& pointData) -> void
{
  m_objects.Add(std::in_place_type<boundary_walls>, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 0.0f, levelIndex);

  m_boundary = std::accumulate(std::begin(pointData), std::end(pointData), RECT_F { 0, 0, 0, 0 }, [](RECT_F bounds, POINT_2F pointData) -> RECT_F
  {
    return { std::min(bounds.left, pointData.x), std::min(bounds.top, pointData.y), std::max(bounds.right, pointData.x), std::max(bounds.bottom, pointData.y) };
  });
}

inline auto level_container::ObjectCount(object_type objectType) const noexcept -> std::size_t
{
  switch( objectType )
  {
    case object_type::power_up:
      return m_objects.PowerUpCount();
    default:
      return 0;
  }
}

inline [[nodiscard]] auto level_container::Boundary() const -> RECT_F
{
  return m_boundary;
}

inline auto level_container::CentrePoint() const noexcept -> POINT_2F
{
  return m_cells->CentrePoint();
}

inline auto level_container::Objects() const noexcept -> const default_object_collection &
{
  return m_objects;
}

inline auto level_container::Objects() noexcept -> default_object_collection &
{
  return m_objects;
}

inline auto level_container::Particles() const noexcept -> const particle_collection &
{
  return m_particles;
}

inline auto level_container::Particles() noexcept -> particle_collection &
{
  return m_particles;
}

auto level_container::UpdateObject(auto &object, float interval) -> void
{
  object.Update(interval);
}

auto level_container::VisitObject(auto& object) -> void
{
}

auto level_container::OnCollision(auto&& object1, auto&& object2, geometry_collision::result result) -> void
{
}

auto level_container::OnCollision(auto &&object, boundary_walls &boundaryWalls, geometry_collision::result result) -> void
{
  if( result != geometry_collision::result::containment )
  {
    object.Destroy();
  }
}
