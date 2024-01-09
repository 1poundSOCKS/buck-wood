#pragma once

#include "level_geometries.h"
#include "level_objects.h"
#include "level_collisions.h"
#include "level_explosion.h"
#include "play_events.h"
#include "reload_counter.h"
#include "targetted_object.h"

#include "collisions/particle_collision_results.h"
#include "collisions/geometry_collision_results.h"
#include "collisions/particle_containment_results.h"
#include "collisions/geometry_containment_results.h"

class level_container
{

public:

  using target_object = dynamic_object<level_target>;
  using duct_fan_object = dynamic_object<duct_fan>;
  using asteroid_object = dynamic_object<level_asteroid>;
  using mine_object = dynamic_object<mine>;

  using target_collection = std::vector<target_object>;
  using duct_fan_collection = std::vector<duct_fan_object>;
  using asteroid_collection = std::vector<asteroid_object>;
  using mine_collection = std::list<mine_object>;

  using bullet_collection = std::list<homing_bullet>;
  using explosion_particle_collection  = std::list<explosion_particle>;
  using impact_particle_collection  = std::list<impact_particle>;
  using thrust_particle_collection = std::list<thrust_particle>;

  using targetted_object_type = std::optional<targetted_object>;

  using explosion_collection = std::vector<game_point>;
  using impact_collection = std::vector<game_point>;

public:

  level_container(std::ranges::input_range auto&& points, play_events playEvents);
  level_container(const level_container& levelContainer) = delete;

  auto AddTargets(std::ranges::input_range auto&& positions) -> void;
  auto AddAsteroids(std::ranges::input_range auto&& asteroids) -> void;
  auto AddDuctFans(std::ranges::input_range auto&& asteroids) -> void;

  auto Update(float interval, D2D1_RECT_F viewRect) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

  [[nodiscard]] auto Targets() const -> const target_collection&;
  [[nodiscard]] auto PlayerPosition() const -> game_point;
  [[nodiscard]] auto PlayerAngle() const -> float;
  [[nodiscard]] auto PlayerHasThrusterOn() const -> bool;
  [[nodiscard]] auto PlayerDied() const -> bool;
  [[nodiscard]] auto PlayerShields() const -> const player_ship::shield_status&;
  [[nodiscard]] auto IsComplete() const -> bool;
  [[nodiscard]] auto HasFinished() const -> bool;
  [[nodiscard]] auto TargettedObject() const -> targetted_object_type;

private:

  auto UpdateObjects(float interval) -> void;
  auto ValidateObjects() -> void;
  auto RemoveDestroyedObjects() -> void;
  auto DoPlayerCollisions() -> void;
  auto DoNonPlayerCollisions() -> void;
  auto ProcessCollisionResults() -> void;
  auto CreateNewObjects(float interval) -> void;
  auto GetTargettedObject() -> targetted_object_type;
  auto GetNearestObject(auto* object1, auto* object2, float maxRange) const -> targetted_object_type;
  auto GetNearestObject(auto* object1, auto* object2) const -> std::tuple<targetted_object_type, float>;
  auto GetNearestToPlayer(auto& mine1, auto& mine2) const -> auto&;
  auto DistanceFromPlayer(auto&& object) const -> float;
  auto GetMaxCollisionCount(int currentMaxCollisionCount) const -> int;

private:

  reload_counter m_thrustEmmisionCounter { 1.0f / 10.0f, 1 };
  reload_counter m_playerReloadCounter { 1.0f / 10.0f, 1 };
  blank_object m_boundary;
  play_events m_playEvents;
  dynamic_object<player_ship> m_playerShip { level_geometries::PlayerShipGeometry(), game_point { 0, 0 } };
  target_collection m_targets;
  mine_collection m_mines;
  duct_fan_collection m_ductFans;
  bullet_collection m_bullets;
  explosion_particle_collection m_explosionParticles;
  impact_particle_collection m_impactParticles;
  thrust_particle_collection m_thrustParticles;
  asteroid_collection m_asteroids;

  particle_containment_results<explosion_particle> m_explosionContainmentResults;
  particle_containment_results<thrust_particle> m_thrustContainmentResults;
  particle_containment_results<bullet> m_bulletContainmentResults;

  geometry_containment_results<mine> m_mineContainmentResults;
  geometry_containment_results<player_ship> m_shipContainmentResults;

  geometry_collision_results<player_ship, level_asteroid> m_shipToAsteroidCollisionResults;
  geometry_collision_results<player_ship, level_target> m_shipToTargetCollisionResults;
  geometry_collision_results<player_ship, duct_fan> m_shipToDuctFanCollisionResults;
  geometry_collision_results<player_ship, mine> m_shipToMineCollisionResults;

  geometry_collision_results<mine, level_asteroid> m_mineToAsteroidCollisionResults;
  geometry_collision_results<mine, duct_fan> m_mineToDuctFanCollisionResults;

  particle_collision_results<player_ship, explosion_particle> m_shipToExplosionCollisionResults;
  particle_collision_results<level_asteroid, explosion_particle> m_asteroidExplosionCollisionResults;
  particle_collision_results<mine, bullet> m_mineToBulletCollisionResults;
  particle_collision_results<level_asteroid, bullet> m_asteroidToBulletCollisionResults;
  particle_collision_results<duct_fan, bullet> m_ductFanToBulletCollisionResults;
  particle_collision_results<level_target, bullet> m_targetToBulletCollisionResults;
  particle_collision_results<duct_fan, explosion_particle> m_ductFanToExplosionCollisionResults;
  particle_collision_results<level_asteroid, thrust_particle> m_asteroidToThrustCollisionResults;
  particle_collision_results<duct_fan, thrust_particle> m_ductFanToThrustCollisionResults;

  explosion_collection m_explosions;
  impact_collection m_impacts;

  int m_activatedTargetCount { 0 };
  targetted_object_type m_targettedObject;

  int m_maxCollisionCount { 0 };

};

level_container::level_container(std::ranges::input_range auto&& points, play_events playEvents) : m_boundary { points }, m_playEvents { playEvents }
{
}

auto level_container::AddTargets(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_targets.emplace_back(level_geometries::TargetGeometry(), position);
  });
}

auto level_container::AddDuctFans(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_ductFans.emplace_back(level_geometries::DuctFanGeometry(), position.x, position.y, 30.0f);
  });
}

auto level_container::AddAsteroids(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_asteroids.emplace_back(level_geometries::AsteroidGeometry(), position.x, position.y, 200.0f, 200.0f);
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

inline [[nodiscard]] auto level_container::PlayerPosition() const -> game_point
{
  return m_playerShip->Position();
}

inline [[nodiscard]] auto level_container::PlayerHasThrusterOn() const -> bool
{
  return m_playerShip->ThrusterOn();
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

auto level_container::GetNearestObject(auto* object1, auto* object2, float maxRange) const -> targetted_object_type
{
  auto [nearestObject, distance] = GetNearestObject(object1, object2);
  return distance < maxRange ? nearestObject : std::nullopt;
}

auto level_container::GetNearestObject(auto* object1, auto* object2) const -> std::tuple<targetted_object_type, float>
{
  if( object1 && object2 )
  {
    auto distance1 = DistanceFromPlayer(*object1);
    auto distance2 = DistanceFromPlayer(*object2);
    return distance2 < distance1 ? std::tuple<targetted_object_type, float> { targetted_object { object2 }, distance2 } : std::tuple<targetted_object_type, float> { targetted_object { object1 }, distance1 };
  }
  else if( object1 )
  {
    return { object1, DistanceFromPlayer(*object1) };
  }
  else if( object2 )
  {
    return { object2, DistanceFromPlayer(*object2) };
  }
  else
  {
    return { std::nullopt, 0.0f };
  }
}

auto level_container::GetNearestToPlayer(auto& object1, auto& object2) const -> auto&
{
  return DistanceFromPlayer(object2) < DistanceFromPlayer(object1) ? object2 : object1;
}

auto level_container::DistanceFromPlayer(auto&& object) const -> float
{
  return m_playerShip->Position().DistanceTo(object->Position());
}
