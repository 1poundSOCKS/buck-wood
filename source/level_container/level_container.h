#pragma once

#include "level_geometries.h"
#include "level_objects.h"
#include "level_collisions.h"
#include "level_explosion.h"
#include "play_events.h"
#include "targetted_object.h"
#include "level_stage.h"
#include "game_score.h"

class level_container
{

public:

  using static_object_collection = std::vector<dynamic_object<default_object>>;
  using moving_object_collection = std::list<dynamic_object<default_object>>;

  using particle_collection = std::list<particle>;

  using explosion_collection = std::vector<D2D1_POINT_2F>;
  using impact_collection = std::vector<D2D1_POINT_2F>;

  level_container(std::ranges::input_range auto&& points, play_events playEvents, std::shared_ptr<game_score> gameScore);
  level_container(const level_container& levelContainer) = delete;

  auto AddTargets(std::ranges::input_range auto&& positions) -> void;
  auto SetPlayerActive(bool value) -> void;

  auto Update(float interval, D2D1_RECT_F viewRect) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

  [[nodiscard]] auto PlayerPosition() const -> std::optional<POINT_2F>;
  [[nodiscard]] auto PlayerAngle() const -> float;
  [[nodiscard]] auto PlayerHasThrusterOn() const -> bool;
  [[nodiscard]] auto PlayerDied() const -> bool;
  [[nodiscard]] auto PlayerShields() const -> const player_ship::shield_status&;
  [[nodiscard]] auto IsComplete() const -> bool;
  [[nodiscard]] auto HasFinished() const -> bool;
  [[nodiscard]] auto TargettedObject() const -> std::optional<targetted_object>;
  [[nodiscard]] auto LevelSize() const -> D2D1_SIZE_F;
  [[nodiscard]] auto GameScore() const -> const game_score&;
  [[nodiscard]] auto MineParameters();

  auto CreateExplosion(D2D1_POINT_2F position) -> void;
  auto CreateParticle(auto&&...args) -> void;
  auto CreateImpact(D2D1_POINT_2F position) -> void;
  auto CreateMovingObject(auto&&...args) -> void;
  auto TargetActivated() -> void;
  auto MineDestroyed() -> void;
  auto SetPlayEvent(auto&&...args) -> void;
  auto UpdatePlayer(player_ship playerShip) -> void;

  static [[nodiscard]] auto ConvertFireModeToDamageMode(player_ship::fire_mode fireMode) -> std::optional<bullet::damage_mode>;

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

  blank_object m_boundary;
  play_events m_playEvents;
  std::optional<player_ship> m_playerShip;
  particle_collection m_particles;

  static_object_collection m_staticObjects;
  moving_object_collection m_movingObjects;

  explosion_collection m_explosions;
  impact_collection m_impacts;

  int m_activatedTargetCount { 0 };
  
  std::optional<targetted_object> m_targettedObject;
  std::optional<D2D1_POINT_2F> m_targetPosition;

  int m_maxCollisionCount { 0 };
  bool m_playerActive { false };

  level_stage m_stage;
  std::shared_ptr<game_score> m_gameScore;

};

level_container::level_container(std::ranges::input_range auto&& points, play_events playEvents, std::shared_ptr<game_score> gameScore) : 
  m_boundary { points }, m_playEvents { playEvents }, m_gameScore { gameScore }
{
  m_playerShip = player_ship { POINT_2F { 0 , 0 } };
  m_movingObjects.emplace_back(level_geometries::PlayerShipGeometry(), std::in_place_type<player_ship>, POINT_2F { 0 , 0 });
}

auto level_container::AddTargets(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_staticObjects.emplace_back(level_geometries::TargetGeometry(), std::in_place_type<level_target>, position, 5.0f);
  });
}

inline [[nodiscard]] auto level_container::PlayerAngle() const -> float
{
  return m_playerShip->Angle();
}

inline [[nodiscard]] auto level_container::PlayerShields() const -> const player_ship::shield_status&
{
  return m_playerShip->ShieldStatus();  
}

inline [[nodiscard]] auto level_container::PlayerPosition() const -> std::optional<POINT_2F>
{
  return m_playerShip->Position();
}

inline [[nodiscard]] auto level_container::PlayerHasThrusterOn() const -> bool
{
  return !m_playerShip->Destroyed() && m_playerShip->ThrusterOn();
}

inline [[nodiscard]] auto level_container::PlayerDied() const -> bool
{
  return m_playerShip->Destroyed();
}

inline [[nodiscard]] auto level_container::IsComplete() const -> bool
{
  return false;
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

inline [[nodiscard]] auto level_container::MineParameters()
{
  return m_stage.MineParameters();
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

inline auto level_container::TargetActivated() -> void
{
  m_playEvents.SetEvent(play_events::event_type::target_activated, true);
}

inline auto level_container::MineDestroyed() -> void
{
  m_gameScore->Add(10);
}

auto level_container::SetPlayEvent(auto&&...args) -> void
{
  m_playEvents.SetEvent(std::forward<decltype(args)>(args)...);
}

inline auto level_container::UpdatePlayer(player_ship playerShip) -> void
{
  m_playerShip = playerShip;
}

auto level_container::GetNearestToTarget(auto& object1, auto& object2) const -> auto&
{
  return DistanceFromTarget(object2) < DistanceFromTarget(object1) ? object2 : object1;
}

auto level_container::DistanceFromTarget(auto&& object) const -> float
{
  return direct2d::GetDistanceBetweenPoints(*m_targetPosition, object->Position());
}

inline [[nodiscard]] auto level_container::ConvertFireModeToDamageMode(player_ship::fire_mode fireMode) -> std::optional<bullet::damage_mode>
{
  switch( fireMode )
  {
    case player_ship::fire_mode::one:
      return bullet::damage_mode::one;
    case player_ship::fire_mode::two:
      return bullet::damage_mode::two;
    default:
      return std::nullopt;
  }
}
