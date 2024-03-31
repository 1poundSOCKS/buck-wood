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

  using StaticObjectAllocator = linear_allocator<dynamic_object<default_object>, size_t { 10 }>;
  using MovingObjectAllocator = linear_allocator<dynamic_object<default_object>, size_t { 50 }>;
  using ParticleAllocator = linear_allocator<particle, size_t { 2000 }>;

  using static_object_collection = std::list<dynamic_object<default_object>, StaticObjectAllocator>;
  using moving_object_collection = std::list<dynamic_object<default_object>, MovingObjectAllocator>;
  using particle_collection = std::list<particle, ParticleAllocator>;

  enum class level_type { vertical_scroller, arena };

  level_container(std::shared_ptr<play_events> playEvents, std::shared_ptr<game_score> gameScore, std::shared_ptr<int> powerUpsCollected);
  level_container(level_type levelType, int index, std::ranges::input_range auto&& points, POINT_2F playerPosition, std::shared_ptr<play_events> playEvents, std::shared_ptr<game_score> gameScore, std::shared_ptr<int> powerUpsCollected);
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

  [[nodiscard]] auto MovingObjects(auto&& unaryFunction);

  auto CreateStaticObject(auto&&...args) -> void;
  auto CreateMovingObject(auto&&...args) -> void;
  auto CreatePortal(auto&&...args) -> void;
  auto CreatePlayer(auto&&...args) -> void;
  auto CreateEnemyType1(auto&&...args) -> void;
  auto CreateEnemyType2(auto&&...args) -> void;
  auto CreateEnemyBullet(auto&&...args) -> void;
  auto CreateParticle(auto&&...args) -> void;
  auto CreatePlayerBullet(auto&&...args) -> void;
  auto CreatePowerUp(auto&&...args) -> void;

  auto CreateExplosion(POINT_2F position) -> void;
  auto CreateImpact(POINT_2F position) -> void;

  auto CreateExplosions(auto&& positions) -> void;
  auto CreateImpacts(auto&& positions) -> void;

  auto Boundary() const -> const blank_object&;
  auto StaticObjects() const -> const static_object_collection&;
  auto MovingObjects() const -> const moving_object_collection&;
  auto Particles() const -> const particle_collection&;

  auto TargetDamaged(const enemy_type_2& target) const -> void;
  auto MineDestroyed() const -> void;
  auto PowerUpCollected(POINT_2F position) -> void;

  auto SavePlayerState(player_ship playerShip) -> void;

  [[nodiscard]] auto GetShipMovementType(level_type levelType) -> player_ship::movement_type;

  auto UpdateObject(player_ship& object, float interval) -> void;
  auto UpdateObject(enemy_type_1& object, float interval) -> void;
  auto UpdateObject(enemy_type_2& object, float interval) -> void;

private:

  auto Update(auto&& updateVisitor, auto&& collisionHandler, D2D1_RECT_F viewRect) -> void;
  auto UpdateObjects(auto&& visitor) -> void;
  auto ValidateObjectPointers() -> void;
  auto RemoveDestroyedObjects() -> void;
  auto DoCollisions(auto&& handler) -> void;
  auto CreateNewObjects(auto&& visitor) -> void;
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

  static_object_collection m_staticObjects;
  moving_object_collection m_movingObjects;

  std::optional<targetted_object> m_targettedObject;

  std::shared_ptr<play_events> m_playEvents;
  std::shared_ptr<game_score> m_gameScore;
  std::shared_ptr<int> m_powerUpsCollected;

  enemy_movement_random m_enemyMovementRandom;

  int m_targetsRemaining { 0 };

};

inline level_container::level_container(std::shared_ptr<play_events> playEvents, std::shared_ptr<game_score> gameScore, std::shared_ptr<int> powerUpsCollected) : 
  level_container(level_type::arena, 0, std::array<POINT_2F, 0>(), { 0, 0 }, playEvents, gameScore, powerUpsCollected)
{
}

inline level_container::level_container(level_type levelType, int index, std::ranges::input_range auto&& points, POINT_2F playerPosition, std::shared_ptr<play_events> playEvents, 
  std::shared_ptr<game_score> gameScore, std::shared_ptr<int> powerUpsCollected) : 
  m_type { levelType }, m_index { index }, m_boundary { points }, m_playerState { GetShipMovementType(levelType), playerPosition }, m_playEvents { playEvents }, 
  m_gameScore { gameScore }, m_powerUpsCollected { powerUpsCollected }
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

inline [[nodiscard]] auto level_container::GameScore() const -> const game_score&
{
  return *m_gameScore;
}

inline [[nodiscard]] auto level_container::TargetCount() const -> int
{
  return m_targetsRemaining;
}

auto level_container::CreateStaticObject(auto&&...args) -> void
{
  m_staticObjects.emplace_back(std::forward<decltype(args)>(args)...);
}

auto level_container::CreateMovingObject(auto&&...args) -> void
{
  m_movingObjects.emplace_back(std::forward<decltype(args)>(args)...);
}

[[nodiscard]] auto level_container::MovingObjects(auto&& unaryFunction)
{
  return std::ranges::views::filter(m_movingObjects, unaryFunction);
}

auto level_container::CreatePortal(auto&&...args) -> void
{
  CreateStaticObject(level_geometries::CircleGeometry(), std::in_place_type<portal>, std::forward<decltype(args)>(args)...);
}

auto level_container::CreatePlayer(auto&&...args) -> void
{
  CreateStaticObject(level_geometries::PlayerShipGeometry(), std::in_place_type<player_ship>, GetShipMovementType(m_type), std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreatePlayerBullet(auto&&...args) -> void
{
  CreateStaticObject(level_geometries::PlayerBulletGeometry(), std::in_place_type<player_bullet>, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateEnemyType1(auto&&...args) -> void
{
  CreateMovingObject(level_geometries::TargetGeometry(), std::in_place_type<enemy_type_1>, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateEnemyType2(auto&&...args) -> void
{
  CreateMovingObject(level_geometries::TargetGeometry(), std::in_place_type<enemy_type_2>, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateEnemyBullet(auto&&...args) -> void
{
  CreateMovingObject(level_geometries::MineGeometry(), std::in_place_type<enemy_bullet_1>, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateParticle(auto&&...args) -> void
{
  m_particles.emplace_back(std::forward<decltype(args)>(args)...);
}

auto level_container::CreatePowerUp(auto&&...args) -> void
{
  CreateMovingObject(level_geometries::CircleGeometry(), std::in_place_type<power_up>, std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreateExplosion(POINT_2F position) -> void
{
  std::ranges::copy(level_explosion { position }, std::back_inserter(m_particles));
}

inline auto level_container::CreateImpact(POINT_2F position) -> void
{
  m_particles.emplace_back(particle::type::impact, position, VELOCITY_2F { 0, 0 }, 0.5f);
}

auto level_container::CreateExplosions(auto&& positions) -> void
{
  for( const auto& position : positions )
  {
    std::ranges::copy(level_explosion { position }, std::back_inserter(m_particles));
  }
}

auto level_container::CreateImpacts(auto&& positions) -> void
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

inline auto level_container::StaticObjects() const -> const static_object_collection&
{
  return m_staticObjects;
}

inline auto level_container::MovingObjects() const -> const moving_object_collection&
{
  return m_movingObjects;
}

inline auto level_container::Particles() const -> const particle_collection&
{
  return m_particles;
}

inline auto level_container::TargetDamaged(const enemy_type_2& object) const -> void
{
  if( object.Destroyed() )
  {
    m_gameScore->Add(50);
  }
}

inline auto level_container::MineDestroyed() const -> void
{
  m_gameScore->Add(20);
}

inline auto level_container::PowerUpCollected(POINT_2F position) -> void
{
  m_playEvents->SetEvent(play_events::event_type::power_up_collected, true);
  ++(*m_powerUpsCollected);
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

  for( const auto& object : m_staticObjects )
  {
    auto ship = object->GetIf<player_ship>();
    m_playerState = ship ? *ship : m_playerState;
  }

  RemoveDestroyedObjects();

  m_targettedObject = m_playerState.Destroyed() ? std::nullopt : GetTargettedObject();

  auto enemies = std::ranges::views::transform(m_movingObjects, [](const auto& object)
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

  auto staticObjects = std::ranges::views::filter(m_staticObjects, [](const auto& object) { return !object->Destroyed(); } );

  for( auto& object : staticObjects )
  {
    std::visit(visitor, object->Get());
    object.UpdateGeometry();
  }

  auto movingObjects = std::ranges::views::filter(m_movingObjects, [](const auto& object) { return !object->Destroyed(); } );

  for( auto& object : movingObjects )
  {
    std::visit(visitor, object->Get());
    object.UpdateGeometry();
  }
}

auto level_container::DoCollisions(auto&& handler) -> void
{
  if( m_boundary.Geometry() )
  {
    geometry_containment<default_object> geometryContainmentRunner;
    geometryContainmentRunner(m_boundary.Geometry().get(), m_staticObjects, handler);
    geometryContainmentRunner(m_boundary.Geometry().get(), m_movingObjects, handler);

    particle_containment<particle> particleContainmentRunner;
    particleContainmentRunner(m_boundary.Geometry().get(), m_particles, handler);
  }

  geometry_collision_binary<default_object, default_object> staticMovingCollisionRunner;
  staticMovingCollisionRunner(m_staticObjects, m_movingObjects, handler);

#ifdef SELF_COLLISION
  geometry_collision_unary<default_object> movingCollisionRunner;
  movingCollisionRunner(m_movingObjects, handler);
#endif

#ifdef ALL_PARTICLE_COLLISIONS_TESTED
  particle_collision<default_object, particle> particleCollisionRunner { collisionHandler };
  particleCollisionRunner(m_staticObjects, m_particles);
  particleCollisionRunner(m_movingObjects, m_particles);
#endif
}
