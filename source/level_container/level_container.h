#pragma once

#include "level_geometries.h"
#include "level_objects.h"
#include "level_cell_collection.h"
#include "level_explosion.h"
#include "targetted_object.h"
#include "play_events.h"
#include "game_score.h"
#include "game_settings.h"
#include "linear_allocator.h"
#include "dynamic_object_functions.h"
#include "collisions/geometry_containment.h"
#include "collisions/particle_containment.h"
#include "collisions/geometry_collision_binary.h"
#include "collisions/geometry_collision_unary.h"
#include "collisions/cell_collision_tests.h"

class level_container
{

public:

  using NoninteractiveObjectAllocator = linear_allocator<default_object, size_t { 10 }>;
  using PlayerObjectAllocator = linear_allocator<default_object, size_t { 20 }>;
  using EnemyObjectAllocator = linear_allocator<default_object, size_t { 50 }>;
  using ParticleAllocator = linear_allocator<particle, size_t { 1000 }>;

  using noninteractive_object_collection = std::list<default_object, NoninteractiveObjectAllocator>;
  using player_object_collection = std::list<default_object, PlayerObjectAllocator>;
  using enemy_object_collection = std::list<default_object, EnemyObjectAllocator>;
  using particle_collection = std::list<particle, ParticleAllocator>;

  level_container();
  level_container(const level_container& levelContainer) = delete;

  auto AddFloorCell(int x, int y, level_cell_type cellType) -> void;
  auto AddWalls() -> void;

  auto Update(float interval, D2D1_RECT_F viewRect) -> void;

  [[nodiscard]] auto PlayerDestroyed() const noexcept -> bool;
  [[nodiscard]] auto PlayerPosition() const noexcept -> POINT_2F;
  [[nodiscard]] auto PlayerThrusterOn() const noexcept -> bool;

  [[nodiscard]] auto TargettedObject() const -> std::optional<targetted_object>;
  [[nodiscard]] auto LevelSize() const -> D2D1_SIZE_F;
  [[nodiscard]] auto EnemyCount() const -> enemy_object_collection::size_type;

  auto EnumerateCells(auto&& visitor) const -> void;
  auto EnumerateNonInteractiveObjects(auto&& visitor) const -> void;
  auto EnumerateNonInteractiveObjects(auto&& visitor) -> void;
  auto EnumerateParticles(auto&& visitor) const -> void;
  auto EnumerateEnemies(auto&& visitor) const -> void;  
  auto EnumerateAllObjects(bool includeDestroyedObjects, auto&& visitor) -> void;
  auto EnumerateInteractiveObjects(auto&& visitor) const -> void;

  [[nodiscard]] auto Exit() const noexcept -> bool;
  [[nodiscard]] auto ExitCell() const noexcept -> POINT_2I;
  auto SetExit(bool value, POINT_2I cell) -> void;

  auto CreateNoninteractiveObject(auto variantType, POINT_2F position, auto&&...args) -> void;
  auto CreatePlayerObject(auto variantType, POINT_2F position, auto&&...args) -> void;
  auto CreateEnemyObject(auto variantType, POINT_2F position, auto&&...args) -> void;

  auto CreatePlayer(POINT_2I cell) -> void;
  auto CreatePlayerBullet(auto&&...args) -> void;
  auto CreatePortal(POINT_2I cell) -> void;
  auto CreateBackgroundObject(auto&&...args) -> void;
  auto CreateEnemyType1(POINT_2I cell, auto&&...args) -> void;
  auto CreateEnemyType2(POINT_2I cell, auto&&...args) -> void;
  auto CreateEnemyType3(POINT_2I cell, auto&&...args) -> void;
  auto CreateEnemyBullet(POINT_2F position, auto&&...args) -> void;
  auto CreateParticle(auto&&...args) -> void;
  auto CreatePlayerMissile(auto&&...args) -> void;
  auto CreatePowerUp(POINT_2F position, auto&&...args) -> void;

  auto CreateExplosion(POINT_2F position) -> void;
  auto CreateImpact(POINT_2F position) -> void;

  auto CreateExplosions(std::ranges::input_range auto&& positions) -> void;
  auto CreateImpacts(std::ranges::input_range auto&& positions) -> void;

  auto SavePlayerState(player_ship playerShip) -> void;

private:

  auto UpdateObjects(float interval) -> void;
  auto UpdateObject(player_ship& object, float interval) -> void;
  auto UpdateObject(player_missile& object, float interval) -> void;
  auto UpdateObject(enemy_type_1& object, float interval) -> void;
  auto UpdateObject(enemy_type_2& object, float interval) -> void;
  auto UpdateObject(enemy_type_3& object, float interval) -> void;
  auto ValidateObjectPointers() -> void;
  auto RemoveDestroyedObjects() -> void;
  auto DoCollisions() -> void;
  auto UpdateObject(auto &object, float interval) -> void;
  // auto GetTargettedObject() -> std::optional<targetted_object>;
  auto GetNearestToTarget(auto& mine1, auto& mine2) const -> auto&;
  auto DistanceFromTarget(auto&& object) const -> float;

private:

  static constexpr float m_maxTargetRange { 1000.0f };

  level_cell_collection m_cells;

  bool m_exit { false };
  POINT_2I m_exitCell { 0, 0 };

  particle_collection m_particles;

  player_ship m_playerState;

  noninteractive_object_collection m_noninteractiveObjects;
  player_object_collection m_playerObjects;
  enemy_object_collection m_enemyObjects;

  std::optional<targetted_object> m_targettedObject;

  particle_containment m_particleContainmentRunner;
  geometry_collision_binary m_collisionRunner;
  cell_collision_tests m_cellCollisionTests;

  enemy_object_collection::size_type m_enemyCount { 0 };

};

inline level_container::level_container() : m_cells { 400, 400 }, m_playerState { { 0, 0} }
{
}

inline [[nodiscard]] auto level_container::PlayerDestroyed() const noexcept -> bool
{
  return m_playerState.Destroyed();
}

inline [[nodiscard]] auto level_container::PlayerPosition() const noexcept -> POINT_2F
{
  return m_playerState.Position();
}

inline [[nodiscard]] auto level_container::PlayerThrusterOn() const noexcept -> bool
{
  return m_playerState.ThrusterOn();
}

inline [[nodiscard]] auto level_container::TargettedObject() const -> std::optional<targetted_object>
{
  return m_targettedObject;
}

inline [[nodiscard]] auto level_container::LevelSize() const -> D2D1_SIZE_F
{
  return { 4000, 4000 };
}

inline [[nodiscard]] auto level_container::EnemyCount() const -> enemy_object_collection::size_type
{
  return m_enemyCount;
}

auto level_container::CreateNoninteractiveObject(auto variantType, POINT_2F position, auto&&...args) -> void
{
  m_noninteractiveObjects.emplace_back(variantType, position, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateEnemyObject(auto variantType, POINT_2F position, auto&&...args) -> void
{
  m_enemyObjects.emplace_back(variantType, position, std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreatePlayerObject(auto variantType, POINT_2F position, auto &&...args) -> void
{
  m_playerObjects.emplace_back(variantType, position, std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreatePortal(POINT_2I cell) -> void
{
  auto cellPosition = m_cells.CellPosition(cell.x, cell.y);
  m_playerState.SetPosition(cellPosition);
  CreateNoninteractiveObject(std::in_place_type<portal>, cellPosition);
}

auto level_container::CreateBackgroundObject(auto&&...args) -> void
{
  CreateNoninteractiveObject(level_geometries::AsteroidGeometry(), std::in_place_type<background_object>, std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreatePlayerMissile(auto&&...args) -> void
{
  CreatePlayerObject(level_geometries::MineGeometry(), std::in_place_type<player_missile>, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateEnemyType1(POINT_2I cell, auto&&...args) -> void
{
  auto cellPosition = m_cells.CellPosition(cell.x, cell.y);
  CreateEnemyObject(std::in_place_type<enemy_type_1>, cellPosition, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateEnemyType2(POINT_2I cell, auto&&...args) -> void
{
  auto cellPosition = m_cells.CellPosition(cell.x, cell.y);
  CreateEnemyObject(std::in_place_type<enemy_type_2>, cellPosition, std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreateEnemyType3(POINT_2I cell, auto &&...args) -> void
{
  auto cellPosition = m_cells.CellPosition(cell.x, cell.y);
  CreateEnemyObject(std::in_place_type<enemy_type_3>, cellPosition, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateEnemyBullet(POINT_2F position, auto&&...args) -> void
{
  CreateEnemyObject(std::in_place_type<enemy_bullet_1>, position, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateParticle(auto&&...args) -> void
{
  m_particles.emplace_back(std::forward<decltype(args)>(args)...);
}

auto level_container::CreatePowerUp(POINT_2F position, auto&&...args) -> void
{
  CreateEnemyObject(std::in_place_type<power_up>, position, std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreatePlayer(POINT_2I cell) -> void
{
  auto cellPosition = m_cells.CellPosition(cell.x, cell.y);
  CreatePlayerObject(std::in_place_type<player_ship>, cellPosition);
}

inline auto level_container::CreatePlayerBullet(auto &&...args) -> void
{
  CreatePlayerObject(std::in_place_type<player_bullet>, std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreateExplosion(POINT_2F position) -> void
{
  std::ranges::copy(level_explosion { position }, std::back_inserter(m_particles));
  play_events::set(play_events::event_type::explosion, true);
}

inline auto level_container::CreateImpact(POINT_2F position) -> void
{
  m_particles.emplace_back(particle::type::impact, position, VELOCITY_2F { 0, 0 }, 0.5f);
}

auto level_container::CreateExplosions(std::ranges::input_range auto&& positions) -> void
{
  for( const auto& position : positions )
  {
    std::ranges::copy(level_explosion { position }, std::back_inserter(m_particles));
  }
}

auto level_container::CreateImpacts(std::ranges::input_range auto&& positions) -> void
{
  for( const auto& position : positions )
  {
    m_particles.emplace_back(particle::type::impact, position, VELOCITY_2F { 0, 0 }, 0.5f);
  }
}

auto level_container::EnumerateCells(auto &&visitor) const -> void
{
  m_cells.EnumerateCells(visitor);
}

auto level_container::EnumerateNonInteractiveObjects(auto &&visitor) const -> void
{
  for( const auto& object : m_noninteractiveObjects )
  {
    visitor(object);
  }
}

inline auto level_container::EnumerateNonInteractiveObjects(auto &&visitor) -> void
{
  for( auto& object : m_noninteractiveObjects )
  {
    visitor(object);
  }
}

inline auto level_container::EnumerateParticles(auto &&visitor) const -> void
{
  for( const auto& particle : m_particles )
  {
    visitor(particle);
  }
}

inline auto level_container::EnumerateEnemies(auto &&visitor) const -> void
{
  auto enemies = std::ranges::views::filter(m_enemyObjects, [this](const auto& object)
  {
    return object.HoldsAlternative<enemy_type_1>() || object.HoldsAlternative<enemy_type_2>() || object.HoldsAlternative<enemy_type_3>();
  });

  for( const auto& object : enemies )
  {
    visitor(object);
  }
}

inline auto level_container::EnumerateAllObjects(bool includeDestroyedObjects, auto &&visitor) -> void
{
  auto noninteractiveObjects = std::ranges::views::filter(m_noninteractiveObjects, [includeDestroyedObjects](const auto& object)
  {
    return includeDestroyedObjects || !object.Destroyed();
  });

  for( auto& object : noninteractiveObjects )
  {
    visitor(object);
  }

  auto playerObjects = std::ranges::views::filter(m_playerObjects, [includeDestroyedObjects](const auto& object)
  {
    return includeDestroyedObjects || !object.Destroyed();
  });

  for( auto& object : playerObjects )
  {
    visitor(object);
  }

  auto enemyObjects = std::ranges::views::filter(m_enemyObjects, [includeDestroyedObjects](const auto& object)
  {
    return includeDestroyedObjects || !object.Destroyed();
  });

  for( auto& object : enemyObjects )
  {
    visitor(object);
  }
}

inline auto level_container::EnumerateInteractiveObjects(auto &&visitor) const -> void
{
  for( const auto& object : m_playerObjects )
  {
    visitor(object);
  }

  for( const auto& object : m_enemyObjects )
  {
    visitor(object);
  }
}

inline auto level_container::Exit() const noexcept -> bool
{
  return m_exit;
}

inline auto level_container::ExitCell() const noexcept -> POINT_2I
{
  return m_exitCell;
}

inline auto level_container::SetExit(bool value, POINT_2I cell) -> void
{
  m_exit = true;
  m_exitCell = cell;
}

inline auto level_container::SavePlayerState(player_ship playerState) -> void
{
  m_playerState = playerState;
}

auto level_container::GetNearestToTarget(auto& object1, auto& object2) const -> auto&
{
  return DistanceFromTarget(object2) < DistanceFromTarget(object1) ? object2 : object1;
}

auto level_container::DistanceFromTarget(auto&& object) const -> float
{
  return direct2d::GetDistanceBetweenPoints(m_playerState.Position(), object->Position());
}

auto level_container::UpdateObject(auto& object, float interval) -> void
{
  object.Update(interval);
}
