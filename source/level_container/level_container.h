#pragma once

#include "level_geometries.h"
#include "level_objects.h"
#include "level_collisions.h"

#include "level_explosion.h"

#include "level_input.h"
#include "play_events.h"

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

public:

  level_container(std::ranges::input_range auto&& points, play_events playEvents);
  level_container(const level_container& levelContainer) = delete;

  auto AddTargets(std::ranges::input_range auto&& positions) -> void;
  auto AddAsteroids(std::ranges::input_range auto&& asteroids) -> void;
  auto AddDuctFans(std::ranges::input_range auto&& asteroids) -> void;

  auto Update(int64_t ticks, D2D1_RECT_F viewRect) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

  [[nodiscard]] auto Targets() const -> const target_collection&;
  [[nodiscard]] auto PlayerPosition() const -> game_point;
  [[nodiscard]] auto PlayerAngle() const -> float;
  [[nodiscard]] auto PlayerHasThrusterOn() const -> bool;
  [[nodiscard]] auto PlayerDied() const -> bool;
  [[nodiscard]] auto PlayerShields() const -> const player_ship::shield_status&;
  [[nodiscard]] auto IsComplete() const -> bool;
  [[nodiscard]] auto HasFinished() const -> bool;
  [[nodiscard]] auto TargettedObject() const -> std::optional<mine>;

private:

  auto UpdateObjects(float interval) -> void;
  auto EraseDestroyedObjects() -> void;
  auto DoPlayerCollisions() -> void;
  auto DoNonPlayerCollisions() -> void;
  auto CreateNewObjects(float interval) -> void;
  auto GetTargettedObject() const -> std::optional<mine>;
  auto GetNearest(const mine& mine1, const mine& mine2) const -> const mine&;

private:

  reload_timer m_thrustEmmisionTimer { 1.0f / 10.0f };
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

  level_collision_checks m_collisionChecks;
  level_containment_checks m_containmentChecks;
  
  int m_activatedTargetCount { 0 };
  std::optional<mine> m_targettedObject;

};

level_container::level_container(std::ranges::input_range auto&& points, play_events playEvents) : m_boundary { points }, m_playEvents { playEvents }
{
}

auto level_container::AddTargets(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_targets.emplace_back(level_geometries::TargetGeometry(), position.x, position.y);
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

inline [[nodiscard]] auto level_container::TargettedObject() const -> std::optional<mine>
{
  return m_targettedObject;
}
