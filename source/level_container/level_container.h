#pragma once

#include "level_data_types.h"
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

  level_container(std::ranges::input_range auto&& points, play_events playEvents);
  level_container(const level_container& levelContainer) = delete;

  auto AddTargets(std::ranges::input_range auto&& positions) -> void;
  auto AddAsteroids(std::ranges::input_range auto&& asteroids) -> void;
  auto AddDuctFans(std::ranges::input_range auto&& asteroids) -> void;

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

private:

  auto UpdateObjects(float interval) -> void;
  auto ValidateObjects() -> void;
  auto RemoveDestroyedObjects() -> void;
  auto DoPlayerCollisions() -> void;
  auto DoNonPlayerCollisions() -> void;
  auto CreateNewObjects(float interval) -> void;
  auto GetTargettedObject() -> targetted_object_type;
  auto GetNearestObject(auto* object1, auto* object2, float maxRange) const -> targetted_object_type;
  auto GetNearestObject(auto* object1, auto* object2) const -> std::tuple<targetted_object_type, float>;
  auto GetNearestToPlayer(auto& mine1, auto& mine2) const -> auto&;
  auto DistanceFromPlayer(auto&& object) const -> float;
  template <typename geometry_object_type> auto DestroyObjectsOnGeometryCollision(std::ranges::input_range auto&& objects) -> void;
  template <typename geometry_object_type> auto DestroyObjectOnGeometryCollision(auto& object) -> void;
  template <typename particle_object_type> auto DestroyParticlesOnGeometryCollision(std::ranges::input_range auto&& particles) -> void;
  auto DestroyBulletsOnGeometryCollision(std::ranges::input_range auto&& bullets) -> void;

private:

  reload_counter m_thrustEmmisionCounter { 1.0f / 10.0f, 1 };
  reload_counter m_playerReloadCounter { 1.0f / 10.0f, 1 };
  blank_object m_boundary;
  play_events m_playEvents;
  dynamic_object<player_ship> m_playerShip { level_geometries::PlayerShipGeometry(), D2D1_POINT_2F { 0, 0 } };
  target_collection m_targets;
  mine_collection m_mines;
  duct_fan_collection m_ductFans;
  bullet_collection m_bullets;
  explosion_particle_collection m_explosionParticles;
  impact_particle_collection m_impactParticles;
  thrust_particle_collection m_thrustParticles;
  asteroid_collection m_asteroids;

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
  return direct2d::GetDistanceBetweenPoints(m_playerShip->Position(), object->Position());
}

template <typename particle_object_type> auto level_container::DestroyParticlesOnGeometryCollision(std::ranges::input_range auto&& particles) -> void
{
  particle_containment<particle_object_type> destroyParticlesAtBoundary { [this](auto& particle)
  {
    particle.Destroy();
  }};

  particle_collision<level_asteroid, particle_object_type> destroyParticlesOnAsteroids { [this](auto& asteroid, auto& particle)
  {
    particle.Destroy();
  }};

  particle_collision<duct_fan, particle_object_type> destroyParticlesOnDuctFans { [this](auto& ductFan, auto& particle)
  {
    particle.Destroy();
  }};

  destroyParticlesAtBoundary(m_boundary, particles);
  destroyParticlesOnAsteroids(m_asteroids, particles);
  destroyParticlesOnDuctFans(m_ductFans, particles);
}

template <typename geometry_object_type> auto level_container::DestroyObjectsOnGeometryCollision(std::ranges::input_range auto&& objects) -> void
{
  geometry_containment<geometry_object_type> destroyObjectsAtBoundary { [this](auto& object)
  {
    m_explosions.emplace_back(object->PreviousPosition());
    object->Destroy();
  }};

  geometry_collision<level_asteroid, geometry_object_type> destroyObjectsOnAsteroids { [this](auto& asteroid, auto& object)
  {
    m_explosions.emplace_back(object.PreviousPosition());
    object.Destroy();
  }};

  geometry_collision<duct_fan, geometry_object_type> destroyObjectsOnDuctFans { [this](auto& ductFan, auto& object)
  {
    m_explosions.emplace_back(object.PreviousPosition());
    object.Destroy();
  }};

  destroyObjectsAtBoundary(m_boundary, objects);
  destroyObjectsOnAsteroids(m_asteroids, objects);
  destroyObjectsOnDuctFans(m_ductFans, objects);
}

template <typename geometry_object_type> auto level_container::DestroyObjectOnGeometryCollision(auto& object) -> void
{
  geometry_containment<geometry_object_type> destroyObjectAtBoundary { [this](auto& object)
  {
    m_explosions.emplace_back(object->PreviousPosition());
    object->Destroy();
  }};

  geometry_collision<geometry_object_type, level_asteroid> destroyObjectOnAsteroids { [this](auto& object, auto& asteroid)
  {
    m_explosions.emplace_back(object.PreviousPosition());
    object.Destroy();
  }};

  geometry_collision<geometry_object_type, duct_fan> destroyObjectOnDuctFans { [this](auto& object, auto& ductFan)
  {
    m_explosions.emplace_back(object.PreviousPosition());
    object.Destroy();
  }};

  destroyObjectAtBoundary(m_boundary, object);
  destroyObjectOnAsteroids(object, m_asteroids);
  destroyObjectOnDuctFans(object, m_ductFans);
}
