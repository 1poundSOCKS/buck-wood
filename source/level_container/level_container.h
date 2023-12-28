#pragma once

#include "level_geometries.h"
#include "level_objects.h"
#include "level_collisions.h"

#include "level_explosion.h"

#include "dynamic_object_collection.h"
#include "particle_collection.h"

#include "level_input.h"

class level_container
{

public:

  struct update_events
  {
    bool playerShot { false };
    bool targetActivated { false };
    bool mineExploded { false };
  };

  using target_object = dynamic_object<level_target>;
  using mine_object = dynamic_object<mine>;
  using duct_fan_object = dynamic_object<duct_fan>;

  using target_collection = dynamic_object_collection<level_target>;
  using mine_collection = dynamic_object_collection<mine>;
  using duct_fan_collection = dynamic_object_collection<duct_fan>;
  using asteroid_collection = dynamic_object_collection<level_asteroid>;

  using bullet_collection = particle_collection<bullet>;
  using explosion_particle_collection  = particle_collection<explosion_particle>;
  using impact_particle_collection  = particle_collection<impact_particle>;
  using thrust_particle_collection = particle_collection<thrust_particle>;

public:

  level_container(std::ranges::input_range auto&& points);
  level_container(const level_container& levelContainer) = delete;

  auto AddTargets(std::ranges::input_range auto&& positions) -> void;
  auto AddAsteroids(std::ranges::input_range auto&& asteroids) -> void;
  auto AddDuctFans(std::ranges::input_range auto&& asteroids) -> void;

  auto Update(const level_input& input, int64_t ticks, D2D1_RECT_F viewRect) -> update_events;
  auto Render(D2D1_RECT_F viewRect) const -> void;

  [[nodiscard]] auto Targets() const -> const target_collection&;
  [[nodiscard]] auto PlayerPosition() const -> game_point;
  [[nodiscard]] auto PlayerAngle() const -> float;
  [[nodiscard]] auto PlayerHasThrusterOn() const -> bool;
  [[nodiscard]] auto PlayerDied() const -> bool;
  [[nodiscard]] auto PlayerShields() const -> const player_ship::shield_status&;
  [[nodiscard]] auto IsComplete() const -> bool;
  [[nodiscard]] auto HasFinished() const -> bool;
  [[nodiscard]] auto CalculateTargettedMine() const -> std::optional<mine>;

private:

  auto UpdateObjects(float interval, std::optional<game_point> playerPosition, std::optional<game_point> targetPosition) -> void;
  auto EraseDestroyedObjects() -> void;
  auto DoPlayerCollisions() -> void;
  auto DoNonPlayerCollisions() -> void;
  auto CreateNewObjects(float interval, const std::optional<game_point>& playerPosition) -> void;
  auto GetNearest(const mine& mine1, const mine& mine2) const -> const mine&;

private:

  reload_timer m_thrustEmmisionTimer { 1.0f / 10.0f };

  blank_object m_boundary;
  dynamic_object<player_ship> m_playerShip { level_geometries::PlayerShipGeometry(), game_point { 0, 0 } };
  target_collection m_targets;
  mine_collection m_mines;
  duct_fan_collection m_ductFans;
  bullet_collection m_bullets;
  explosion_particle_collection m_explosionParticles;
  impact_particle_collection m_impactParticles;
  thrust_particle_collection m_thrustParticles;  
  asteroid_collection m_asteroids;

  int m_activatedTargetCount { 0 };

  level_collision_checks m_collisionChecks;
  level_containment_checks m_containmentChecks;

};

level_container::level_container(std::ranges::input_range auto&& points) : m_boundary { points }
{
}

auto level_container::AddTargets(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_targets.Create(level_geometries::TargetGeometry(), position.x, position.y);
  });
}

auto level_container::AddDuctFans(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_ductFans.Create(level_geometries::DuctFanGeometry(), position.x, position.y, 30.0f);
  });
}

auto level_container::AddAsteroids(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_asteroids.Create(level_geometries::AsteroidGeometry(), position.x, position.y, 200.0f, 200.0f);
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
  return m_activatedTargetCount == m_targets.Size();
}

inline [[nodiscard]] auto level_container::HasFinished() const -> bool
{
  return PlayerDied() || IsComplete();
}
