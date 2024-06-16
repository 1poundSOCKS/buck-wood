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

class level_container
{

public:

  enum class object_type { portal_entry, portal_exit, player, enemy_stalker, enemy_random, enemy_turret, power_up, cell };

  level_container();
  level_container(collision_type collisionType);
  level_container(const level_container& levelContainer) = delete;

  auto Create(object_type objectType, POINT_2F position, SCALE_2F scale, float angle) -> default_object&;
  auto Update(float interval, D2D1_RECT_F viewRect) -> void;

  [[nodiscard]] auto PlayerThrusterOn() const noexcept -> bool;
  [[nodiscard]] auto PlayerState() const noexcept -> const player_ship_state&;

  [[nodiscard]] auto LevelSize() const -> SIZE_F;
  [[nodiscard]] auto EnemyCount() const -> size_t;

  auto EnumerateObjects(auto&& visitor) const -> void;
  auto EnumerateParticles(auto&& visitor) const -> void;
  auto EnumerateEnemyObjects(auto&& visitor) const -> void;

  [[nodiscard]] auto Exit() const noexcept -> bool;
  [[nodiscard]] auto ExitCell() const noexcept -> cell_id;
  auto SetExit(bool value, cell_id cell) -> void;

private:

  auto VisitObject(player_ship& object) -> void;
  auto VisitObject(enemy_type_1& object) -> void;
  auto VisitObject(enemy_type_2& object) -> void;
  auto VisitObject(enemy_type_3& object) -> void;
  auto VisitObject(auto &object) -> void;

  auto DoCollisions() -> void;
  auto DoCollisions(std::ranges::input_range auto &&objectGeometries1, std::ranges::input_range auto &&objectGeometries2, bool checkContainment) -> void;

  auto OnCollision(player_bullet& bullet, enemy_type_1& enemy) -> void;
  auto OnCollision(player_bullet& bullet, enemy_type_2& enemy) -> void;
  auto OnCollision(player_bullet& bullet, enemy_type_3& enemy) -> void;
  auto OnCollision(player_bullet& bullet, level_cell& wall) -> void;
  auto OnCollision(enemy_bullet_1& bullet, level_cell& wall) -> void;
  auto OnCollision(player_ship& playerShip, enemy_type_1& enemy) -> void;
  auto OnCollision(player_ship& playerShip, enemy_type_2& enemy) -> void;
  auto OnCollision(player_ship& playerShip, enemy_type_3& enemy) -> void;
  auto OnCollision(player_ship& playerShip, enemy_bullet_1& enemyBullet) -> void;
  auto OnCollision(player_ship& playerShip, power_up& powerUp) -> void;
  auto OnCollision(auto&& object1, auto&& object2) -> void;

  auto OnContainment(player_ship& player, portal& portalObj) -> void;
  auto OnContainment(auto&& object1, auto&& object2) -> void;

private:

  bool m_exit { false };
  cell_id m_exitCell;

  std::shared_ptr<player_ship_state> m_playerState;

  default_object_collection m_objects;
  particle_collection m_particles;

  particle_collision m_particleCollisionRunner;
  range_comparison_runner m_compare;
  geometry_collision m_collisionTest;
  geometry_containment m_containmentTest;

  level_collision_geometry m_collisionGeometry;

  size_t m_enemyCount { 0 };
};

inline [[nodiscard]] auto level_container::PlayerThrusterOn() const noexcept -> bool
{
  return m_playerState->ThrusterOn();
}

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

inline auto level_container::EnumerateEnemyObjects(auto &&visitor) const -> void
{
  for( const auto& object : m_objects )
  {
    visitor(object);
  }
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

auto level_container::VisitObject(auto& object) -> void
{
}

auto level_container::DoCollisions(std::ranges::input_range auto &&objectGeometries1, std::ranges::input_range auto &&objectGeometries2, bool checkContainment) -> void
{
  m_compare(objectGeometries1, objectGeometries2, [this,checkContainment](auto& object1, auto& object2)
  {
    if( m_collisionTest(object1, object2) )
    {
      object1.Object().Visit([this,&object2](auto& levelObject1)
      {
        object2.Object().Visit([this,&levelObject1](auto& levelObject2) { OnCollision(levelObject1, levelObject2); } );
      });
    }

    if( checkContainment && m_containmentTest(object1, object2) )
    {
      object1.Object().Visit([this,&object2](auto& levelObject1)
      {
        object2.Object().Visit([this,&levelObject1](auto& levelObject2) { OnContainment(levelObject1, levelObject2); } );
      });
    }
  });
}

auto level_container::OnCollision(auto&& object1, auto&& object2) -> void
{
}

auto level_container::OnContainment(auto&& object1, auto&& object2) -> void
{
}
