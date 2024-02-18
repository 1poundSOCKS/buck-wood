#pragma once

#include "level_data_types.h"
#include "level_geometries.h"
#include "level_objects.h"
#include "level_collisions.h"
#include "level_explosion.h"
#include "play_events.h"
#include "targetted_object.h"
#include "level_stage.h"

class level_container
{

public:

  using target_collection = std::vector<target_object>;
  using duct_fan_collection = std::vector<duct_fan_object>;
  using asteroid_collection = std::vector<asteroid_object>;
  using mine_collection = std::list<mine_object>;

  using bullet_collection = std::list<homing_bullet>;
  using explosion_particle_collection  = std::list<explosion_particle>;
  using impact_particle_collection  = std::list<impact_particle>;
  using thrust_particle_collection = std::list<thrust_particle>;

  using particle_collection = std::list<particle>;

  using explosion_collection = std::vector<D2D1_POINT_2F>;
  using impact_collection = std::vector<D2D1_POINT_2F>;

  level_container(std::ranges::input_range auto&& points, play_events playEvents);
  level_container(const level_container& levelContainer) = delete;

  auto AddTargets(std::ranges::input_range auto&& positions) -> void;
  auto AddAsteroids(std::ranges::input_range auto&& asteroids) -> void;
  auto AddDuctFans(std::ranges::input_range auto&& asteroids) -> void;
  auto SetPlayerActive(bool value) -> void;

  auto Update(float interval, D2D1_RECT_F viewRect) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

  [[nodiscard]] auto Targets() const -> const target_collection&;
  [[nodiscard]] auto PlayerPosition() const -> D2D1_POINT_2F;
  [[nodiscard]] auto PlayerAngle() const -> float;
  [[nodiscard]] auto PlayerHasThrusterOn() const -> bool;
  [[nodiscard]] auto PlayerDied() const -> bool;
  [[nodiscard]] auto PlayerShields() const -> const player_ship::shield_status&;
  [[nodiscard]] auto IsComplete() const -> bool;
  [[nodiscard]] auto HasFinished() const -> bool;
  [[nodiscard]] auto TargettedObject() const -> targetted_object_type;
  [[nodiscard]] auto LevelSize() const -> D2D1_SIZE_F;

  auto CreateExplosion(D2D1_POINT_2F position) -> void;
  auto CreateImpact(D2D1_POINT_2F position) -> void;
  auto TargetActivated() -> void;

private:

  auto UpdateObjects(float interval) -> void;
  auto ValidateObjectPointers() -> void;
  auto RemoveDestroyedObjects() -> void;
  auto DoCollisions() -> void;
  auto CreateNewObjects(float interval) -> void;
  auto GetTargettedObject() -> targetted_object_type;
  auto GetNearestObject(auto* object1, auto* object2, float maxRange) const -> targetted_object_type;
  auto GetNearestObject(auto* object1, auto* object2) const -> std::tuple<targetted_object_type, float>;
  auto GetNearestToTarget(auto& mine1, auto& mine2) const -> auto&;
  auto DistanceFromTarget(auto&& object) const -> float;
  static [[nodiscard]] auto ConvertFireModeToDamageMode(player_ship::fire_mode fireMode) -> std::optional<bullet::damage_mode>;

private:

  static constexpr float m_maxTargetRange { 1000.0f };

  blank_object m_boundary;
  play_events m_playEvents;
  dynamic_object<player_ship> m_playerShip { level_geometries::PlayerShipGeometry(), D2D1_POINT_2F { 0, 0 } };
  target_collection m_targets;
  mine_collection m_mines;
  duct_fan_collection m_ductFans;
  bullet_collection m_bullets;
  asteroid_collection m_asteroids;
  particle_collection m_particles;

  explosion_collection m_explosions;
  impact_collection m_impacts;

  int m_activatedTargetCount { 0 };
  targetted_object_type m_targettedObject;
  std::optional<D2D1_POINT_2F> m_targetPosition;

  int m_maxCollisionCount { 0 };
  bool m_playerActive { false };

  level_stage m_stage;

};

level_container::level_container(std::ranges::input_range auto&& points, play_events playEvents) : m_boundary { points }, m_playEvents { playEvents }
{
}

auto level_container::AddTargets(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_targets.emplace_back(level_geometries::TargetGeometry(), position, 5.0f);
  });
}

auto level_container::AddDuctFans(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_ductFans.emplace_back(level_geometries::DuctFanGeometry(), position, 30.0f);
  });
}

auto level_container::AddAsteroids(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_asteroids.emplace_back(level_geometries::AsteroidGeometry(), position, 200.0f, 200.0f);
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

inline[[nodiscard]] auto level_container::Targets() const -> const target_collection&
{
  return m_targets;
}

inline [[nodiscard]] auto level_container::PlayerPosition() const -> D2D1_POINT_2F
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
  return m_activatedTargetCount == m_targets.size();
}

inline [[nodiscard]] auto level_container::HasFinished() const -> bool
{
  return PlayerDied() || IsComplete();
}

inline [[nodiscard]] auto level_container::TargettedObject() const -> targetted_object_type
{
  return m_targettedObject;
}

inline [[nodiscard]] auto level_container::LevelSize() const -> D2D1_SIZE_F
{
  return direct2d::GetGeometrySize(m_boundary);
}

inline auto level_container::CreateExplosion(D2D1_POINT_2F position) -> void
{
  m_explosions.emplace_back(position);
}

inline auto level_container::CreateImpact(D2D1_POINT_2F position) -> void
{
  m_impacts.emplace_back(position);
}

inline auto level_container::TargetActivated() -> void
{
  m_playEvents.SetEvent(play_events::event_type::target_activated, true);
}

auto level_container::GetNearestObject(auto* object1, auto* object2, float maxRange) const -> targetted_object_type
{
  auto [nearestObject, distance] = GetNearestObject(object1, object2);
  return distance < maxRange ? nearestObject : std::nullopt;
}

auto level_container::GetNearestObject(auto* object1, auto* object2) const -> std::tuple<targetted_object_type, float>
{
  if( object1 && object2 )
  {
    auto distance1 = DistanceFromTarget(*object1);
    auto distance2 = DistanceFromTarget(*object2);
    return distance2 < distance1 ? std::tuple<targetted_object_type, float> { targetted_object { object2 }, distance2 } : std::tuple<targetted_object_type, float> { targetted_object { object1 }, distance1 };
  }
  else if( object1 )
  {
    return { object1, DistanceFromTarget(*object1) };
  }
  else if( object2 )
  {
    return { object2, DistanceFromTarget(*object2) };
  }
  else
  {
    return { std::nullopt, 0.0f };
  }
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
