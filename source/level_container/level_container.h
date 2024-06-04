#pragma once

#include "framework.h"
#include "default_object.h"
#include "level_cell_collection.h"
#include "level_explosion.h"
#include "targetted_object.h"
#include "play_events.h"
#include "game_score.h"
#include "game_settings.h"
#include "collisions/particle_collision.h"
#include "collisions/cell_collision_tests.h"
#include "collisions/collision_object.h"
#include "collisions/range_comparision_runner.h"
#include "collisions/geometry_collision.h"
#include "collisions/geometry_containment.h"

class level_container
{

public:

  enum class object_type { portal_entry, portal_exit, player, enemy_stalker, enemy_random, enemy_turret, power_up };

  level_container();
  level_container(collision_type collisionType);
  level_container(const level_container& levelContainer) = delete;

  auto Update(float interval, D2D1_RECT_F viewRect) -> void;

  [[nodiscard]] auto PlayerDestroyed() const noexcept -> bool;
  [[nodiscard]] auto PlayerPosition() const noexcept -> POINT_2F;
  [[nodiscard]] auto PlayerAngle() const noexcept -> float;
  [[nodiscard]] auto PlayerThrusterOn() const noexcept -> bool;

  [[nodiscard]] auto TargettedObject() const -> std::optional<targetted_object>;
  [[nodiscard]] auto LevelSize() const -> SIZE_F;
  [[nodiscard]] auto EnemyCount() const -> size_t;

  auto EnumerateNonInteractiveObjects(auto&& visitor) const -> void;
  auto EnumerateNonInteractiveObjects(auto&& visitor) -> void;

  auto EnumeratePlayerObjects(bool includeDestroyedObjects, auto&& visitor) -> void;
  auto EnumerateEnemyObjects(bool includeDestroyedObjects, auto&& visitor) -> void;
  auto EnumerateEnemies(bool includeDestroyedObjects, auto&& visitor) const -> void;
  auto EnumerateAllObjects(bool includeDestroyedObjects, auto&& visitor) -> void;
  auto EnumerateInteractiveObjects(auto&& visitor) const -> void;
  auto EnumerateParticles(auto&& visitor) const -> void;

  auto EnumerateEnemyCollisionObjects(auto&& visitor) const -> void;
  auto EnumerateFloorCollisionObjects(auto&& visitor) const -> void;
  auto EnumerateExitCollisionObjects(auto&& visitor) const -> void;

  [[nodiscard]] auto Exit() const noexcept -> bool;
  [[nodiscard]] auto ExitCell() const noexcept -> POINT_2I;
  auto SetExit(bool value, POINT_2I cell) -> void;

  auto Create(object_type objectType, POINT_2F position) -> default_object&;
  auto CreateWall(POINT_2F position, SCALE_2F scale, float angle, level_cell_type cellType, POINT_2I cellId) -> void;

  auto SavePlayerState(player_ship playerShip) -> void;

private:

  auto CreateNoninteractiveObject(auto variantType, POINT_2F position, SCALE_2F scale, float angle) -> default_object&;
  auto CreateWallObject(POINT_2F position, SCALE_2F scale, float angle, level_cell_type cellType, POINT_2I cellId) -> void;
  auto CreatePlayerObject(auto variantType, POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) -> default_object&;
  auto CreateEnemyObject(auto variantType, POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) -> default_object&;

  auto CreatePlayerBullet(POINT_2F position, SCALE_2F scale, float angle, float speed) -> default_object&;
  auto CreateEnemyBullet(POINT_2F position, SCALE_2F scale, float angle, float speed) -> default_object&;
  auto CreateParticle(auto&&...args) -> void;

  auto CreateExplosion(POINT_2F position) -> void;
  auto CreateImpact(POINT_2F position) -> void;

  auto UpdateObjects(float interval) -> void;

  auto UpdateObject(player_ship& object, float interval) -> void;
  auto UpdateObject(enemy_type_1& object, float interval) -> void;
  auto UpdateObject(enemy_type_2& object, float interval) -> void;
  auto UpdateObject(enemy_type_3& object, float interval) -> void;
  auto UpdateObject(auto &object, float interval) -> void;

  auto ValidateObjectPointers() -> void;
  auto RemoveDestroyedObjects() -> void;
  auto DoCollisions() -> void;

  template <typename object_type_1, typename object_type_2> auto OnCollision(default_object& object1, default_object& object2) -> void;
  auto OnCollision(player_bullet& bullet, enemy_type_1& enemy) -> void;
  auto OnCollision(player_bullet& bullet, enemy_type_2& enemy) -> void;
  auto OnCollision(player_bullet& bullet, enemy_type_3& enemy) -> void;
  auto OnCollision(player_bullet& bullet, level_wall& wall) -> void;
  auto OnCollision(enemy_bullet_1& bullet, level_wall& wall) -> void;
  auto OnCollision(player_ship& playerShip, enemy_type_1& enemy) -> void;
  auto OnCollision(player_ship& playerShip, enemy_type_2& enemy) -> void;
  auto OnCollision(player_ship& playerShip, enemy_type_3& enemy) -> void;
  auto OnCollision(player_ship& playerShip, enemy_bullet_1& enemyBullet) -> void;
  auto OnCollision(player_ship& playerShip, power_up& powerUp) -> void;
  auto OnCollision(auto&& object1, auto&& object2) -> void;

  template <typename object_type_1, typename object_type_2> auto OnContainment(default_object& object1, default_object& object2) -> void;
  auto OnContainment(player_ship& player, portal& portalObj) -> void;
  auto OnContainment(auto&& object1, auto&& object2) -> void;

#if 0
  auto GetTargettedObject() -> std::optional<targetted_object>;
  auto GetNearestToTarget(auto& mine1, auto& mine2) const -> auto&;
  auto DistanceFromTarget(auto&& object) const -> float;
#endif

  auto AddCellCollisionObject(default_object& object, level_wall& cellObject) -> void;
  auto AddCellCollisionObject(default_object& object, auto& cellObject) -> void;

private:

  using DefaultObjectAllocator = linear_allocator_2<default_object>;
  using noninteractive_object_collection = std::list<default_object, DefaultObjectAllocator>;
  using wall_object_collection = std::list<default_object, DefaultObjectAllocator>;
  using player_object_collection = std::list<default_object, DefaultObjectAllocator>;
  using enemy_object_collection = std::list<default_object, DefaultObjectAllocator>;

  linear_allocator_state m_defaultObjectBuffer;
  DefaultObjectAllocator m_defaultObjectAllocator { m_defaultObjectBuffer };

  using ParticleAllocator = linear_allocator_2<particle>;
  using particle_collection = std::list<particle, ParticleAllocator>;

  linear_allocator_state m_particleBuffer;
  ParticleAllocator m_particleAllocator { m_particleBuffer };

  static constexpr float m_maxTargetRange { 1000.0f };

  bool m_exit { false };
  POINT_2I m_exitCell { 0, 0 };

  player_ship m_playerState;

  noninteractive_object_collection m_noninteractiveObjects;
  wall_object_collection m_wallObjects;
  player_object_collection m_playerObjects;
  enemy_object_collection m_enemyObjects;
  particle_collection m_particles;

  using collision_object_collection = std::vector<collision_object>;

  collision_object_collection m_wallCollisionObjects;
  collision_object_collection m_floorCollisionObjects;
  collision_object_collection m_exitCollisionObjects;
  collision_object_collection m_playerCollisionObjects;
  collision_object_collection m_enemyCollisionObjects;

  particle_collision m_particleCollisionRunner;
  range_comparison_runner m_compare;
  geometry_collision m_collisionTest;
  geometry_containment m_containmentTest;

  enemy_object_collection::size_type m_enemyCount { 0 };

  std::optional<targetted_object> m_targettedObject;
};

inline [[nodiscard]] auto level_container::PlayerDestroyed() const noexcept -> bool
{
  return m_playerState.Destroyed();
}

inline [[nodiscard]] auto level_container::PlayerPosition() const noexcept -> POINT_2F
{
  return m_playerState.Position();
}

inline auto level_container::PlayerAngle() const noexcept -> float
{
  return m_playerState.Angle();
}

inline [[nodiscard]] auto level_container::PlayerThrusterOn() const noexcept -> bool
{
  return m_playerState.ThrusterOn();
}

inline [[nodiscard]] auto level_container::TargettedObject() const -> std::optional<targetted_object>
{
  return m_targettedObject;
}

inline [[nodiscard]] auto level_container::LevelSize() const -> SIZE_F
{
  return { 4000, 4000 };
}

inline [[nodiscard]] auto level_container::EnemyCount() const -> enemy_object_collection::size_type
{
  return m_enemyCount;
}

auto level_container::CreateParticle(auto&&...args) -> void
{
  m_particles.emplace_back(std::forward<decltype(args)>(args)...);
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

inline auto level_container::EnumeratePlayerObjects(bool includeDestroyedObjects, auto &&visitor) -> void
{
  auto objects = std::ranges::views::filter(m_playerObjects, [includeDestroyedObjects](const auto& object)
  {
    return includeDestroyedObjects || !object.Destroyed();
  });

  for( auto& object : objects )
  {
    visitor(object);
  }
}

inline auto level_container::EnumerateEnemyObjects(bool includeDestroyedObjects, auto &&visitor) -> void
{
  auto objects = std::ranges::views::filter(m_enemyObjects, [includeDestroyedObjects](const auto& object)
  {
    return includeDestroyedObjects || !object.Destroyed();
  });

  for( auto& object : objects )
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

inline auto level_container::EnumerateEnemies(bool includeDestroyedObjects, auto &&visitor) const -> void
{
  auto enemies = std::ranges::views::filter(m_enemyObjects, [this](const auto& object)
  {
    return object.HoldsAlternative<enemy_type_1>() || object.HoldsAlternative<enemy_type_2>() || object.HoldsAlternative<enemy_type_3>();
  });

  auto objects = std::ranges::views::filter(enemies, [includeDestroyedObjects](const auto& object)
  {
    return includeDestroyedObjects || !object.Destroyed();
  });

  for( const auto& object : objects )
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

inline auto level_container::EnumerateEnemyCollisionObjects(auto &&visitor) const -> void
{
  for( const auto& object : m_enemyCollisionObjects )
  {
    visitor(object);
  }
}

inline auto level_container::EnumerateFloorCollisionObjects(auto &&visitor) const -> void
{
  for( const auto& object : m_floorCollisionObjects )
  {
    visitor(object);
  }
}

inline auto level_container::EnumerateExitCollisionObjects(auto &&visitor) const -> void
{
  for( const auto& object : m_exitCollisionObjects )
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

#if 0
auto level_container::GetNearestToTarget(auto& object1, auto& object2) const -> auto&
{
  return DistanceFromTarget(object2) < DistanceFromTarget(object1) ? object2 : object1;
}

auto level_container::DistanceFromTarget(auto&& object) const -> float
{
  return direct2d::GetDistanceBetweenPoints(m_playerState.Position(), object->Position());
}
#endif

inline auto level_container::AddCellCollisionObject(default_object& object, auto &cellObject) -> void
{
}

auto level_container::UpdateObject(auto& object, float interval) -> void
{
  object.Update(interval);
}

template <typename object_type_1, typename object_type_2> auto level_container::OnCollision(default_object& object1, default_object& object2) -> void
{
  if( std::holds_alternative<object_type_1>(object1.Get()) && std::holds_alternative<object_type_2>(object2.Get()) )
  {
    return OnCollision(std::get<object_type_1>(object1.Get()), std::get<object_type_2>(object2.Get()));
  }

  if( std::holds_alternative<object_type_1>(object2.Get()) && std::holds_alternative<object_type_2>(object1.Get()) )
  {
    return OnCollision(std::get<object_type_1>(object2.Get()), std::get<object_type_2>(object1.Get()));
  }
}

template <typename object_type_1, typename object_type_2> auto level_container::OnContainment(default_object& object1, default_object& object2) -> void
{
  if( std::holds_alternative<object_type_1>(object1.Get()) && std::holds_alternative<object_type_2>(object2.Get()) )
  {
    return OnContainment(std::get<object_type_1>(object1.Get()), std::get<object_type_2>(object2.Get()));
  }

  if( std::holds_alternative<object_type_1>(object2.Get()) && std::holds_alternative<object_type_2>(object1.Get()) )
  {
    return OnContainment(std::get<object_type_1>(object2.Get()), std::get<object_type_2>(object1.Get()));
  }
}

auto level_container::OnCollision(auto&& object1, auto&& object2) -> void
{
}

auto level_container::OnContainment(auto&& object1, auto&& object2) -> void
{
}
