#pragma once

#include "level_geometries.h"
#include "level_objects.h"
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

  enum class level_type { vertical_scroller, arena };

  level_container();
  level_container(level_type levelType, int index, std::ranges::input_range auto&& points, POINT_2F playerPosition);
  level_container(const level_container& levelContainer) = delete;

  auto SetPlayerActive(bool value) -> void;

  auto Update(float interval, D2D1_RECT_F viewRect) -> void;

  [[nodiscard]] auto Type() const -> level_type;
  [[nodiscard]] auto Index() const -> int;
  [[nodiscard]] auto PlayerState() const -> const player_ship&;
  [[nodiscard]] auto PlayerActive() const -> bool;

  [[nodiscard]] auto PlayerInvulnerable() const -> bool;
  auto SetPlayerInvulnerable(bool value) -> void;

  [[nodiscard]] auto TargettedObject() const -> std::optional<targetted_object>;
  [[nodiscard]] auto LevelSize() const -> D2D1_SIZE_F;
  [[nodiscard]] auto GameScore() const -> const game_score&;
  [[nodiscard]] auto TargetCount() const -> int;

  auto Boundary() const -> const blank_object&;
  auto NoninteractiveObjects() const -> const noninteractive_object_collection&;
  auto PlayerObjects() const -> const player_object_collection&;
  auto EnemyObjects() const -> const enemy_object_collection&;
  auto Particles() const -> const particle_collection&;

  [[nodiscard]] auto EnemyObjects(auto&& unaryFunction);

  auto CreateNoninteractiveObject(auto&&...args) -> void;
  auto CreatePlayerObject(auto&&...args) -> void;
  auto CreateEnemyObject(auto&&...args) -> void;

  auto CreatePortal(auto&&...args) -> void;
  auto CreatePlayer(auto&&...args) -> void;
  auto CreateEnemyType1(auto&&...args) -> void;
  auto CreateEnemyType2(auto&&...args) -> void;
  auto CreateEnemyBullet(auto&&...args) -> void;
  auto CreateParticle(auto&&...args) -> void;
  auto CreatePlayerBullet(auto&&...args) -> void;
  auto CreatePlayerMissile(auto&&...args) -> void;
  auto CreatePowerUp(auto&&...args) -> void;

  auto CreateExplosion(POINT_2F position) -> void;
  auto CreateImpact(POINT_2F position) -> void;

  auto CreateExplosions(std::ranges::input_range auto&& positions) -> void;
  auto CreateImpacts(std::ranges::input_range auto&& positions) -> void;

  auto SavePlayerState(player_ship playerShip) -> void;

  [[nodiscard]] auto GetShipMovementType(level_type levelType) -> player_ship::movement_type;

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

  level_type m_type;
  int m_index;

  blank_object m_boundary;
  particle_collection m_particles;

  player_ship m_playerState;
  bool m_playerActive { false };
  bool m_playerInvulnerable { false };

  noninteractive_object_collection m_noninteractiveObjects;
  player_object_collection m_playerObjects;
  enemy_object_collection m_enemyObjects;

  std::optional<targetted_object> m_targettedObject;

  geometry_containment m_geometryContainmentRunner;
  particle_containment m_particleContainmentRunner;
  geometry_collision_binary m_collisionRunner;

  int m_targetsRemaining { 0 };

};

inline level_container::level_container() : 
  level_container(level_type::arena, 0, std::array<POINT_2F, 0>(), { 0, 0 })
{
}

inline level_container::level_container(level_type levelType, int index, std::ranges::input_range auto&& points, POINT_2F playerPosition) : 
  m_type { levelType }, m_index { index }, m_boundary { points }, m_playerState { GetShipMovementType(levelType), playerPosition }
{
}

inline auto level_container::SetPlayerActive(bool value) -> void
{
  m_playerActive = value;
}

inline [[nodiscard]] auto level_container::Type() const -> level_type
{
  return m_type;
}

inline [[nodiscard]] auto level_container::Index() const -> int
{
  return m_index;
}

inline [[nodiscard]] auto level_container::PlayerState() const -> const player_ship&
{
  return m_playerState;
}

inline [[nodiscard]] auto level_container::PlayerActive() const -> bool
{
  return m_playerActive;
}

inline [[nodiscard]] auto level_container::PlayerInvulnerable() const -> bool
{
  return m_playerInvulnerable;
}

inline auto level_container::SetPlayerInvulnerable(bool value) -> void
{
  m_playerInvulnerable = value;
}

inline [[nodiscard]] auto level_container::TargettedObject() const -> std::optional<targetted_object>
{
  return m_targettedObject;
}

inline [[nodiscard]] auto level_container::LevelSize() const -> D2D1_SIZE_F
{
  return m_boundary.Geometry() ? direct2d::GetGeometrySize(m_boundary) : render_target::get()->GetSize();
}

inline [[nodiscard]] auto level_container::TargetCount() const -> int
{
  return m_targetsRemaining;
}

auto level_container::CreateNoninteractiveObject(auto&&...args) -> void
{
  m_noninteractiveObjects.emplace_back(std::forward<decltype(args)>(args)...);
}

auto level_container::CreatePlayerObject(auto&&...args) -> void
{
  m_playerObjects.emplace_back(std::forward<decltype(args)>(args)...);
}

auto level_container::CreateEnemyObject(auto&&...args) -> void
{
  m_enemyObjects.emplace_back(std::forward<decltype(args)>(args)...);
}

auto level_container::CreatePortal(auto&&...args) -> void
{
  CreateNoninteractiveObject(level_geometries::CircleGeometry(), std::in_place_type<portal>, std::forward<decltype(args)>(args)...);
}

auto level_container::CreatePlayer(auto&&...args) -> void
{
  CreatePlayerObject(level_geometries::PlayerShipGeometry(), std::in_place_type<player_ship>, GetShipMovementType(m_type), std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreatePlayerBullet(auto&&...args) -> void
{
  CreatePlayerObject(level_geometries::PlayerBulletGeometry(), std::in_place_type<player_bullet>, std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreatePlayerMissile(auto&&...args) -> void
{
  CreatePlayerObject(level_geometries::PlayerBulletGeometry(), std::in_place_type<player_missile>, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateEnemyType1(auto&&...args) -> void
{
  CreateEnemyObject(level_geometries::TargetGeometry(), std::in_place_type<enemy_type_1>, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateEnemyType2(auto&&...args) -> void
{
  CreateEnemyObject(level_geometries::TargetGeometry(), std::in_place_type<enemy_type_2>, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateEnemyBullet(auto&&...args) -> void
{
  CreateEnemyObject(level_geometries::MineGeometry(), std::in_place_type<enemy_bullet_1>, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateParticle(auto&&...args) -> void
{
  m_particles.emplace_back(std::forward<decltype(args)>(args)...);
}

auto level_container::CreatePowerUp(auto&&...args) -> void
{
  CreateEnemyObject(level_geometries::CircleGeometry(), std::in_place_type<power_up>, std::forward<decltype(args)>(args)...);
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

inline auto level_container::Boundary() const -> const blank_object&
{
  return m_boundary;
}

inline auto level_container::NoninteractiveObjects() const -> const noninteractive_object_collection&
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

[[nodiscard]] auto level_container::EnemyObjects(auto&& unaryFunction)
{
  return std::ranges::views::filter(m_enemyObjects, unaryFunction);
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

inline [[nodiscard]] auto level_container::GetShipMovementType(level_type levelType) -> player_ship::movement_type
{
  switch( levelType )
  {
    case level_container::level_type::vertical_scroller:
      return player_ship::movement_type::horizontal;
    case level_container::level_type::arena:
      return player_ship::movement_type::both;
    default:
      return player_ship::movement_type::both;
  }
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

  m_targettedObject = m_playerState.TargettingActive() ? GetTargettedObject() : std::nullopt;

  auto enemies = std::ranges::views::transform(m_enemyObjects, [](const auto& object)
  {
    return std::holds_alternative<enemy_type_1>(object->Get()) || std::holds_alternative<enemy_type_2>(object->Get()) ? 1 : 0;
  });

  m_targetsRemaining = std::accumulate(std::begin(enemies), std::end(enemies), 0);

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
  if( m_boundary.Geometry() )
  {
    m_geometryContainmentRunner(m_boundary.Geometry().get(), m_playerObjects, handler);
    m_geometryContainmentRunner(m_boundary.Geometry().get(), m_enemyObjects, handler);
    m_particleContainmentRunner(m_boundary.Geometry().get(), m_particles, handler);
  }

  m_collisionRunner(m_playerObjects, m_enemyObjects, handler);
}
