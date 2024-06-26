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

class level_container
{

public:

  enum class object_type { portal_entry, portal_exit, player, enemy_stalker, enemy_random, enemy_turret, power_up, cell };

  level_container();
  level_container(collision_type collisionType);
  level_container(const level_container& levelContainer) = delete;

  auto AddObject(object_type objectType, cell_id cellId) -> default_object&;
  auto AddCell(cell_id cellId, level_cell_type cellType) -> void;

  auto Update(float interval, D2D1_RECT_F viewRect) -> void;
  auto UpdateVelocity(VELOCITY_2F changeInVelocity, float interval) -> void;

  [[nodiscard]] auto PlayerState() const noexcept -> const player_ship_state&;

  [[nodiscard]] auto LevelSize() const -> SIZE_F;
  [[nodiscard]] auto EnemyCount() const -> size_t;

  auto EnumerateObjects(auto&& visitor) const -> void;
  auto EnumerateParticles(auto&& visitor) const -> void;

  [[nodiscard]] auto Exit() const noexcept -> bool;
  [[nodiscard]] auto ExitCell() const noexcept -> cell_id;
  auto SetExit(bool value, cell_id cell) -> void;

private:

  auto AddObject(object_type objectType, POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) -> default_object&;

  auto SetCellId(portal& object, cell_id cellId) -> void;
  auto SetCellId(auto& object, cell_id cellId) -> void;

  auto UpdateObject(player_ship& object, float interval) -> void;
  auto UpdateObject(enemy_type_1& object, float interval) -> void;
  auto UpdateObject(enemy_type_2& object, float interval) -> void;
  auto UpdateObject(enemy_type_3& object, float interval) -> void;
  auto UpdateObject(auto& object, float interval) -> void;

  auto VisitObject(player_ship& object) -> void;
  auto VisitObject(enemy_type_1& object) -> void;
  auto VisitObject(enemy_type_2& object) -> void;
  auto VisitObject(enemy_type_3& object) -> void;
  auto VisitObject(auto &object) -> void;

  auto DoCollisions() -> void;

  auto OnCollision(player_bullet& bullet, enemy_type_1& enemy, geometry_collision::result result) -> void;
  auto OnCollision(player_bullet& bullet, enemy_type_2& enemy, geometry_collision::result result) -> void;
  auto OnCollision(player_bullet& bullet, enemy_type_3& enemy, geometry_collision::result result) -> void;
  auto OnCollision(player_bullet& bullet, level_cell& wall, geometry_collision::result result) -> void;
  auto OnCollision(enemy_bullet_1& bullet, level_cell& wall, geometry_collision::result result) -> void;
  auto OnCollision(player_ship& playerShip, enemy_type_1& enemy, geometry_collision::result result) -> void;
  auto OnCollision(player_ship& playerShip, enemy_type_2& enemy, geometry_collision::result result) -> void;
  auto OnCollision(player_ship& playerShip, enemy_type_3& enemy, geometry_collision::result result) -> void;
  auto OnCollision(player_ship& playerShip, enemy_bullet_1& enemyBullet, geometry_collision::result result) -> void;
  auto OnCollision(player_ship& playerShip, power_up& powerUp, geometry_collision::result result) -> void;
  auto OnCollision(player_ship& player, portal& portalObj, geometry_collision::result result) -> void;
  auto OnCollision(auto&& object1, auto&& object2, geometry_collision::result result) -> void;

private:

  static constexpr int m_cellSize { 250 };
  static constexpr auto cellWidth { static_cast<float>(m_cellSize) };
  static constexpr auto cellHeight { static_cast<float>(m_cellSize) };

  std::shared_ptr<level_cell_collection> m_cells;
  std::shared_ptr<level_object_movement> m_objectMovement;

  bool m_exit { false };
  cell_id m_exitCell;

  std::shared_ptr<player_ship_state> m_playerState;

  default_object_collection m_objects;

  level_collision_geometry m_collisionGeometry;
  geometry_collision_runner m_collisionRunner;

  particle_collection m_particles;

  particle_collision m_particleCollisionRunner;

  size_t m_enemyCount { 0 };
};

inline auto level_container::PlayerState() const noexcept -> const player_ship_state &
{
  return *m_playerState;
}

inline [[nodiscard]] auto level_container::LevelSize() const -> SIZE_F
{
  return { 4000, 4000 };
}

inline [[nodiscard]] auto level_container::EnemyCount() const -> size_t
{
  return m_enemyCount;
}

inline auto level_container::EnumerateObjects(auto &&visitor) const -> void
{
  auto objects = std::ranges::views::filter(m_objects, [](const auto& object)
  {
    return !object.Destroyed();
  });

  for( auto& object : objects )
  {
    visitor(object);
  }
}

inline auto level_container::EnumerateParticles(auto &&visitor) const -> void
{
  m_particles.Visit(visitor);
}

inline auto level_container::Exit() const noexcept -> bool
{
  return m_exit;
}

inline auto level_container::ExitCell() const noexcept -> cell_id
{
  return m_exitCell;
}

inline auto level_container::SetExit(bool value, cell_id cell) -> void
{
  m_exit = true;
  m_exitCell = cell;
}

inline auto level_container::SetCellId(portal &object, cell_id cellId) -> void
{
  object.SetCellId(cellId);
}

auto level_container::SetCellId(auto &object, cell_id cellId) -> void
{
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
