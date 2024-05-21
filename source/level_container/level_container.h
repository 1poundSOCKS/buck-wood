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

  using NoninteractiveObjectAllocator = linear_allocator<dynamic_object<default_object>, size_t { 10 }>;
  using PlayerObjectAllocator = linear_allocator<dynamic_object<default_object>, size_t { 20 }>;
  using EnemyObjectAllocator = linear_allocator<dynamic_object<default_object>, size_t { 50 }>;
  using ParticleAllocator = linear_allocator<particle, size_t { 1000 }>;

  using noninteractive_object_collection = std::list<dynamic_object<default_object>, NoninteractiveObjectAllocator>;
  using player_object_collection = std::list<dynamic_object<default_object>, PlayerObjectAllocator>;
  using enemy_object_collection = std::list<dynamic_object<default_object>, EnemyObjectAllocator>;
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

  [[nodiscard]] auto Cells() const -> const level_cell_collection&;
  [[nodiscard]] auto NoninteractiveObjects() const -> const noninteractive_object_collection&;
  [[nodiscard]] auto PlayerObjects() const -> const player_object_collection&;
  [[nodiscard]] auto EnemyObjects() const -> const enemy_object_collection&;
  [[nodiscard]] auto Particles() const -> const particle_collection&;

  auto EnumerateCells(auto&& visitor) -> void;
  [[nodiscard]] auto EnemyObjects(auto&& unaryFunction);
  
  [[nodiscard]] auto Exit() const noexcept -> bool;
  [[nodiscard]] auto ExitCell() const noexcept -> POINT_2I;
  auto SetExit(bool value, POINT_2I cell) -> void;

  auto CreateNoninteractiveObject(auto&&...args) -> void;
  auto CreatePlayerObject(auto&&...args) -> void;
  auto CreateEnemyObject(winrt::com_ptr<ID2D1Geometry> sourceGeometry, auto variantType, POINT_2F position, auto&&...args) -> void;

  auto CreatePortal(POINT_2I cell) -> void;
  auto CreatePlayer(POINT_2I cell) -> void;
  auto CreateBackgroundObject(auto&&...args) -> void;
  auto CreateEnemyType1(POINT_2I cell, auto&&...args) -> void;
  auto CreateEnemyType2(POINT_2I cell, auto&&...args) -> void;
  auto CreateEnemyBullet(POINT_2F position, auto&&...args) -> void;
  auto CreateParticle(auto&&...args) -> void;
  auto CreatePlayerBullet(auto&&...args) -> void;
  auto CreatePlayerMissile(auto&&...args) -> void;
  auto CreatePowerUp(POINT_2F position, auto&&...args) -> void;

  auto CreateExplosion(POINT_2F position) -> void;
  auto CreateImpact(POINT_2F position) -> void;

  auto CreateExplosions(std::ranges::input_range auto&& positions) -> void;
  auto CreateImpacts(std::ranges::input_range auto&& positions) -> void;

  auto SavePlayerState(player_ship playerShip) -> void;

  auto UpdateObject(player_ship& object, float interval) -> void;
  auto UpdateObject(player_missile& object, float interval) -> void;
  auto UpdateObject(enemy_type_1& object, float interval) -> void;
  auto UpdateObject(enemy_type_2& object, float interval) -> void;

private:

  auto Update(auto&& updateVisitor, auto&& collisionHandler, D2D1_RECT_F viewRect) -> void;
  auto UpdateObjects(auto&& visitor) -> void;
  auto ValidateObjectPointers() -> void;
  auto RemoveDestroyedObjects() -> void;
  auto DoCollisions(auto&& handler) -> void;
  auto GetTargettedObject() -> std::optional<targetted_object>;
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

  geometry_containment m_geometryContainmentRunner;
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

auto level_container::CreateNoninteractiveObject(auto&&...args) -> void
{
  m_noninteractiveObjects.emplace_back(std::forward<decltype(args)>(args)...);
}

auto level_container::CreatePlayerObject(auto&&...args) -> void
{
  m_playerObjects.emplace_back(std::forward<decltype(args)>(args)...);
}

auto level_container::CreateEnemyObject(winrt::com_ptr<ID2D1Geometry> sourceGeometry, auto variantType, POINT_2F position, auto&&...args) -> void
{
  m_enemyObjects.emplace_back(sourceGeometry, variantType, position, std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreatePortal(POINT_2I cell) -> void
{
  auto cellPosition = m_cells.CellPosition(cell.x, cell.y);
  m_playerState.SetPosition(cellPosition);
  CreateNoninteractiveObject(level_geometries::CircleGeometry(), std::in_place_type<portal>, cellPosition);
}

inline auto level_container::CreatePlayer(POINT_2I cell) -> void
{
  auto cellPosition = m_cells.CellPosition(cell.x, cell.y);
  CreatePlayerObject(level_geometries::TargetGeometry(), std::in_place_type<player_ship>, cellPosition);
}

auto level_container::CreateBackgroundObject(auto&&...args) -> void
{
  CreateNoninteractiveObject(level_geometries::AsteroidGeometry(), std::in_place_type<background_object>, std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreatePlayerBullet(auto&&...args) -> void
{
  CreatePlayerObject(level_geometries::TargetGeometry(), std::in_place_type<player_bullet>, std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreatePlayerMissile(auto&&...args) -> void
{
  CreatePlayerObject(level_geometries::MineGeometry(), std::in_place_type<player_missile>, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateEnemyType1(POINT_2I cell, auto&&...args) -> void
{
  auto cellPosition = m_cells.CellPosition(cell.x, cell.y);
  CreateEnemyObject(level_geometries::TargetGeometry(), std::in_place_type<enemy_type_1>, cellPosition, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateEnemyType2(POINT_2I cell, auto&&...args) -> void
{
  auto cellPosition = m_cells.CellPosition(cell.x, cell.y);
  CreateEnemyObject(level_geometries::TargetGeometry(), std::in_place_type<enemy_type_2>, cellPosition, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateEnemyBullet(POINT_2F position, auto&&...args) -> void
{
  CreateEnemyObject(level_geometries::MineGeometry(), std::in_place_type<enemy_bullet_1>, position, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateParticle(auto&&...args) -> void
{
  m_particles.emplace_back(std::forward<decltype(args)>(args)...);
}

auto level_container::CreatePowerUp(POINT_2F position, auto&&...args) -> void
{
  CreateEnemyObject(level_geometries::TargetGeometry(), std::in_place_type<power_up>, position, std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreateExplosion(POINT_2F position) -> void
{
  std::ranges::copy(level_explosion { position }, std::back_inserter(m_particles));
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

inline auto level_container::Cells() const -> const level_cell_collection&
{
  return m_cells;
}

inline auto level_container::NoninteractiveObjects() const -> const noninteractive_object_collection &
{
  return m_noninteractiveObjects;
}

inline auto level_container::PlayerObjects() const -> const player_object_collection&
{
  return m_playerObjects;
}

inline auto level_container::EnemyObjects() const -> const enemy_object_collection&
{
  return m_enemyObjects;
}

inline auto level_container::Particles() const -> const particle_collection&
{
  return m_particles;
}

inline auto level_container::EnumerateCells(auto &&visitor) -> void
{
  auto cells = std::ranges::views::transform(m_cells.Get(), [](const auto& cell) -> valid_cell
  {
    const auto& [key, value] = cell;
    return value;
  });

  for( const auto& cell : cells )
  {
    visitor(cell);
  }
}

[[nodiscard]] auto level_container::EnemyObjects(auto &&unaryFunction)
{
  return std::ranges::views::filter(m_enemyObjects, unaryFunction);
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

auto level_container::Update(auto&& updateVisitor, auto&& collisionHandler, D2D1_RECT_F viewRect) -> void
{
  auto collisionsStart = performance_counter::QueryValue();

  DoCollisions(collisionHandler);

  auto collisionsEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"collisions", collisionsEnd - collisionsStart, game_settings::swapChainRefreshRate());

  auto updateStart = performance_counter::QueryValue();
  UpdateObjects(updateVisitor);

  for( const auto& object : m_playerObjects )
  {
    auto ship = object->GetIf<player_ship>();
    m_playerState = ship ? *ship : m_playerState;
  }

  RemoveDestroyedObjects();

  // m_targettedObject = m_playerState.TargettingActive() ? GetTargettedObject() : std::nullopt;

  auto enemies = std::ranges::views::transform(m_enemyObjects, [](const auto& object)
  {
    return std::holds_alternative<enemy_type_1>(object->Get()) || std::holds_alternative<enemy_type_2>(object->Get()) ? 1 : 0;
  });

  m_enemyCount = m_enemyObjects.size();//std::accumulate(std::begin(enemies), std::end(enemies), 0);

  auto updateEnd = performance_counter::QueryValue();
  diagnostics::addTime(L"level_container::update", updateEnd - updateStart, game_settings::swapChainRefreshRate());
}

auto level_container::UpdateObjects(auto&& visitor) -> void
{
  auto particles = std::ranges::views::filter(m_particles, [](const auto& particle) { return !particle.Destroyed(); } );

  dynamic_object_functions::update(particles, visitor.m_interval);

  auto NoninteractiveObjects = std::ranges::views::filter(m_noninteractiveObjects, [](const auto& object) { return !object->Destroyed(); } );

  for( auto& object : NoninteractiveObjects )
  {
    std::visit(visitor, object->Get());
    object.UpdateGeometry();
  }

  auto PlayerObjects = std::ranges::views::filter(m_playerObjects, [](const auto& object) { return !object->Destroyed(); } );

  for( auto& object : PlayerObjects )
  {
    std::visit(visitor, object->Get());
    object.UpdateGeometry();
  }

  auto EnemyObjects = std::ranges::views::filter(m_enemyObjects, [](const auto& object) { return !object->Destroyed(); } );

  for( auto& object : EnemyObjects )
  {
    std::visit(visitor, object->Get());
    object.UpdateGeometry();
  }
}

auto level_container::DoCollisions(auto&& handler) -> void
{
  m_cellCollisionTests.particles(m_cells, m_particles, handler);
  m_cellCollisionTests(m_cells, m_playerObjects, handler);
  m_cellCollisionTests(m_cells, m_enemyObjects, handler);
  m_collisionRunner(m_playerObjects, m_enemyObjects, handler);
}
