#pragma once

#include "level_geometries.h"
#include "level_objects.h"
#include "level_explosion.h"
#include "play_events.h"
#include "targetted_object.h"
#include "level_stage.h"
#include "game_score.h"

class level_container
{

public:

  using static_object_collection = std::list<dynamic_object<default_object>>;
  using moving_object_collection = std::list<dynamic_object<default_object>>;

  using particle_collection = std::list<particle>;

  using explosion_collection = std::vector<D2D1_POINT_2F>;
  using impact_collection = std::vector<D2D1_POINT_2F>;

  level_container(int index, std::ranges::input_range auto&& points, POINT_2F playerPosition, play_events playEvents, std::shared_ptr<game_score> gameScore);
  level_container(const level_container& levelContainer) = delete;

  auto SetPlayerActive(bool value) -> void;

  auto Update(float interval, D2D1_RECT_F viewRect) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

  [[nodiscard]] auto Index() const -> int;
  [[nodiscard]] auto PlayerPosition() const -> std::optional<POINT_2F>;
  [[nodiscard]] auto PlayerAngle() const -> float;
  [[nodiscard]] auto PlayerHasThrusterOn() const -> bool;
  [[nodiscard]] auto PlayerDied() const -> bool;
  [[nodiscard]] auto PlayerShields() const -> const health_status&;
  [[nodiscard]] auto IsComplete() const -> bool;
  [[nodiscard]] auto HasFinished() const -> bool;
  [[nodiscard]] auto TargettedObject() const -> std::optional<targetted_object>;
  [[nodiscard]] auto LevelSize() const -> D2D1_SIZE_F;
  [[nodiscard]] auto GameScore() const -> const game_score&;

  [[nodiscard]] auto MovingObjects(auto&& unaryFunction);

  auto CreateTarget(auto&&...args) -> void;
  auto CreateMine(POINT_2F position, POINT_2F targetPosition) -> void;
  auto CreateParticle(auto&&...args) -> void;
  auto CreateMovingObject(auto&&...args) -> void;
  auto CreatePlayerBullet(auto&&...args) -> void;
  auto CreateExplosion(D2D1_POINT_2F position) -> void;
  auto CreateImpact(D2D1_POINT_2F position) -> void;

  auto SetPlayEvent(auto&&...args) -> void;

  auto CreateNewObjects(level_target& object) -> void;
  auto CreateNewObjects(player_ship& object) -> void;
  auto CreateNewObjects(auto& object) -> void;

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

  // level_parameters m_levelParameters;
  int m_index;

  blank_object m_boundary;
  play_events m_playEvents;
  particle_collection m_particles;

  std::shared_ptr<player_state> m_playerState;

  static_object_collection m_staticObjects;
  moving_object_collection m_movingObjects;

  explosion_collection m_explosions;
  impact_collection m_impacts;

  std::optional<targetted_object> m_targettedObject;
  std::optional<D2D1_POINT_2F> m_targetPosition;

  level_stage m_stage;
  std::shared_ptr<game_score> m_gameScore;

  int m_targetsRemaining { 0 };

};

level_container::level_container(int index, std::ranges::input_range auto&& points, POINT_2F playerPosition, play_events playEvents, std::shared_ptr<game_score> gameScore) : 
  m_index { index }, m_boundary { points }, m_playerState { std::make_shared<player_state>() }, 
  m_playEvents { playEvents }, m_gameScore { gameScore }
{
  m_playerState->m_position = playerPosition;
  m_movingObjects.emplace_back(level_geometries::PlayerShipGeometry(), std::in_place_type<player_ship>, m_playerState);
}

inline [[nodiscard]] auto level_container::Index() const -> int
{
  return m_index;
}

inline [[nodiscard]] auto level_container::PlayerAngle() const -> float
{
  return m_playerState->m_angle;
}

inline [[nodiscard]] auto level_container::PlayerShields() const -> const health_status&
{
  return m_playerState->m_shieldStatus;
}

inline [[nodiscard]] auto level_container::PlayerPosition() const -> std::optional<POINT_2F>
{
  return m_playerState->m_position;
}

inline [[nodiscard]] auto level_container::PlayerHasThrusterOn() const -> bool
{
  return !m_playerState->m_destroyed && m_playerState->m_thrusterOn;
}

inline [[nodiscard]] auto level_container::PlayerDied() const -> bool
{
  return m_playerState->m_destroyed;
}

inline [[nodiscard]] auto level_container::IsComplete() const -> bool
{
  return m_targetsRemaining == 0;
}

inline [[nodiscard]] auto level_container::HasFinished() const -> bool
{
  return PlayerDied() || IsComplete();
}

inline [[nodiscard]] auto level_container::TargettedObject() const -> std::optional<targetted_object>
{
  return m_targettedObject;
}

inline [[nodiscard]] auto level_container::LevelSize() const -> D2D1_SIZE_F
{
  return direct2d::GetGeometrySize(m_boundary);
}

inline [[nodiscard]] auto level_container::GameScore() const -> const game_score&
{
  return *m_gameScore;
}

[[nodiscard]] auto level_container::MovingObjects(auto&& unaryFunction)
{
  return std::ranges::views::filter(m_movingObjects, unaryFunction);
}

auto level_container::CreateTarget(auto&&...args) -> void
{
  m_staticObjects.emplace_back(level_geometries::TargetGeometry(), std::in_place_type<level_target>, std::forward<decltype(args)>(args)...);
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

auto level_container::CreateMovingObject(auto&&...args) -> void
{
  m_movingObjects.emplace_back(std::forward<decltype(args)>(args)...);
}

inline auto level_container::CreatePlayerBullet(auto&&...args) -> void
{
  CreateMovingObject(level_geometries::PlayerBulletGeometry(), std::in_place_type<player_bullet>, std::forward<decltype(args)>(args)...);
}

auto level_container::SetPlayEvent(auto&&...args) -> void
{
  m_playEvents.SetEvent(std::forward<decltype(args)>(args)...);
}

auto level_container::GetNearestToTarget(auto& object1, auto& object2) const -> auto&
{
  return DistanceFromTarget(object2) < DistanceFromTarget(object1) ? object2 : object1;
}

auto level_container::DistanceFromTarget(auto&& object) const -> float
{
  return direct2d::GetDistanceBetweenPoints(*m_targetPosition, object->Position());
}

auto level_container::CreateNewObjects(auto& object) -> void
{
}
