#pragma once

#include "level_geometries.h"
#include "level_objects.h"
#include "level_explosion.h"
#include "targetted_object.h"
// #include "level_stage.h"
#include "play_events.h"
#include "game_score.h"
#include "linear_allocator.h"

class level_container
{

public:

  using StaticObjectAllocator = linear_allocator<dynamic_object<default_object>, size_t { 10 }>;
  using MovingObjectAllocator = linear_allocator<dynamic_object<default_object>, size_t { 50 }>;
  using ParticleAllocator = linear_allocator<particle, size_t { 2000 }>;

  using static_object_collection = std::list<dynamic_object<default_object>, StaticObjectAllocator>;
  using moving_object_collection = std::list<dynamic_object<default_object>, MovingObjectAllocator>;
  using particle_collection = std::list<particle, ParticleAllocator>;

  using explosion_collection = std::vector<D2D1_POINT_2F>;
  using impact_collection = std::vector<D2D1_POINT_2F>;

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

  auto CreatePortal(auto&&...args) -> void;
  auto CreatePlayer(auto&&...args) -> void;
  auto CreateTarget(auto&&...args) -> void;
  auto CreateMine(auto&&...args) -> void;
  auto CreateParticle(auto&&...args) -> void;
  auto CreateMovingObject(auto&&...args) -> void;
  auto CreatePlayerBullet(auto&&...args) -> void;
  auto CreatePowerUp(auto&&...args) -> void;
  auto CreateEnemyTypeOne(auto&&...args) -> void;

  auto CreateExplosion(D2D1_POINT_2F position) -> void;
  auto CreateImpact(D2D1_POINT_2F position) -> void;

  auto Boundary() const -> const blank_object&;
  auto StaticObjects() const -> const static_object_collection&;
  auto MovingObjects() const -> const moving_object_collection&;
  auto Particles() const -> const particle_collection&;

  auto TargetDamaged(const enemy_type_2& target) const -> void;
  auto MineDestroyed() const -> void;
  auto PowerUpCollected(POINT_2F position) -> void;

  auto SavePlayerState(player_ship playerShip) -> void;

  auto CreateNewObjects(enemy_type_2& object) -> void;
  auto CreateNewObjects(player_ship& object) -> void;
  auto CreateNewObjects(auto& object) -> void;

  [[nodiscard]] auto GetShipMovementType(level_type levelType) -> player_ship::movement_type;

private:

  auto UpdateObjects(float interval) -> void;
  auto ValidateObjectPointers() -> void;
  auto RemoveDestroyedObjects() -> void;
  auto DoCollisions() -> void;
  auto CreateNewObjects(float interval) -> void;
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

  explosion_collection m_explosions;
  impact_collection m_impacts;

  std::optional<targetted_object> m_targettedObject;

  std::shared_ptr<play_events> m_playEvents;
  std::shared_ptr<game_score> m_gameScore;
  std::shared_ptr<int> m_powerUpsCollected;

  int m_targetsRemaining { 0 };

};

inline level_container::level_container(std::shared_ptr<play_events> playEvents, std::shared_ptr<game_score> gameScore, std::shared_ptr<int> powerUpsCollected) : 
  level_container(level_type::arena, 0, std::array<POINT_2F, 0>(), { 0, 0 }, playEvents, gameScore, powerUpsCollected)
{
}

inline level_container::level_container(level_type levelType, int index, std::ranges::input_range auto&& points, POINT_2F playerPosition, std::shared_ptr<play_events> playEvents, 
  std::shared_ptr<game_score> gameScore, std::shared_ptr<int> powerUpsCollected) : 
  m_type { levelType }, m_index { index }, m_boundary { points }, m_playerState { GetShipMovementType(levelType), playerPosition }, m_playEvents { playEvents }, m_gameScore { gameScore }, m_powerUpsCollected { powerUpsCollected }
{
  m_explosions.reserve(10);
  m_impacts.reserve(100);
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

[[nodiscard]] auto level_container::MovingObjects(auto&& unaryFunction)
{
  return std::ranges::views::filter(m_movingObjects, unaryFunction);
}

auto level_container::CreatePortal(auto&&...args) -> void
{
  m_staticObjects.emplace_back(level_geometries::CircleGeometry(), std::in_place_type<portal>, std::forward<decltype(args)>(args)...);
}

auto level_container::CreatePlayer(auto&&...args) -> void
{
  m_movingObjects.emplace_back(level_geometries::PlayerShipGeometry(), std::in_place_type<player_ship>, GetShipMovementType(m_type), std::forward<decltype(args)>(args)...);
}

auto level_container::CreateTarget(auto&&...args) -> void
{
  m_staticObjects.emplace_back(level_geometries::TargetGeometry(), std::in_place_type<enemy_type_2>, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateMine(auto&&...args) -> void
{
  CreateMovingObject(level_geometries::MineGeometry(), std::in_place_type<enemy_bullet_1>, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateEnemyTypeOne(auto&&...args) -> void
{
  CreateMovingObject(level_geometries::TargetGeometry(), std::in_place_type<enemy_type_1>, std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreateExplosion(D2D1_POINT_2F position) -> void
{
  m_explosions.emplace_back(position);
}

auto level_container::CreateParticle(auto&&...args) -> void
{
  m_particles.emplace_back(std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreateImpact(D2D1_POINT_2F position) -> void
{
  m_impacts.emplace_back(position);
}

auto level_container::CreatePowerUp(auto&&...args) -> void
{
  CreateMovingObject(level_geometries::CircleGeometry(), std::in_place_type<power_up>, std::forward<decltype(args)>(args)...);
}

auto level_container::CreateMovingObject(auto&&...args) -> void
{
  m_movingObjects.emplace_back(std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreatePlayerBullet(auto&&...args) -> void
{
  CreateMovingObject(level_geometries::PlayerBulletGeometry(), std::in_place_type<player_bullet>, std::forward<decltype(args)>(args)...);
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

auto level_container::CreateNewObjects(auto& object) -> void
{
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
