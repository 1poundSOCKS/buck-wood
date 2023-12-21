#pragma once

#include "player_ship.h"
#include "level_target.h"
#include "explosion.h"
#include "asteroid_container.h"
#include "mine.h"
#include "explosion_particle.h"
#include "thrust_particle.h"
#include "level_input.h"
#include "solid_objects.h"
#include "blank_objects.h"
#include "duct_fan.h"
#include "dynamic_object_collection.h"
#include "level_geometries.h"
#include "bullet.h"
#include "impact_particle.h"
#include "geometry_collision.h"
#include "particle_collision.h"
#include "geometry_containment.h"
#include "particle_containment.h"
#include "level_collision_checks.h"

class level_container
{

public:

  struct update_events
  {
    auto reset() -> void;

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

  using bullet_collection = std::list<bullet>;  
  using explosion_particle_collection  = std::list<explosion_particle>;
  using impact_particle_collection  = std::list<impact_particle>;
  using thrust_particle_collection = std::list<thrust_particle>;

  level_container() = default;
  level_container(const level_container& levelContainer) = delete;

  auto AddBlankObjects(std::ranges::input_range auto&& objects) -> void;
  auto AddTargets(std::ranges::input_range auto&& positions) -> void;
  auto AddAsteroids(std::ranges::input_range auto&& asteroids) -> void;
  auto AddDuctFans(std::ranges::input_range auto&& asteroids) -> void;

  auto Update(const level_input& input, int64_t ticks, D2D1_RECT_F viewRect) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

  [[nodiscard]] auto Targets() const -> const target_collection&;
  [[nodiscard]] auto PlayerPosition() const -> game_point;
  [[nodiscard]] auto PlayerAngle() const -> float;
  [[nodiscard]] auto PlayerHasThrusterOn() const -> bool;
  [[nodiscard]] auto PlayerDied() const -> bool;
  [[nodiscard]] auto PlayerShields() const -> const player_ship::shield_status&;
  [[nodiscard]] auto IsComplete() const -> bool;
  [[nodiscard]] auto HasFinished() const -> bool;
  [[nodiscard]] auto UpdateEvents() const -> const update_events&;

private:

  auto UpdatePlayer(const level_input& input, float interval) -> void;
  auto DoCollisions() -> void;
  auto CreateExplosion(const game_point& position) -> void;
  auto CreateImpactParticle(const game_point& position) -> void;



private:

  reload_timer m_reloadTimer { 1.0f / 20.0f };
  reload_timer m_thrustEmmisionTimer { 1.0f / 10.0f };

  dynamic_object<player_ship> m_playerShip { level_geometries::PlayerShipGeometry() };
  target_collection m_targets;
  mine_collection m_mines;
  duct_fan_collection m_ductFans;
  bullet_collection m_bullets;
  explosion_particle_collection m_explosionParticles;
  impact_particle_collection m_impactParticles;
  thrust_particle_collection m_thrustParticles;  
  asteroid_collection m_asteroids;
  blank_objects m_blankObjects;

  int m_activatedTargetCount { 0 };
  update_events m_updateEvents;

  level_collision_checks m_collisionChecks {
    [this](const auto& position) { CreateExplosion(position); },
    [this](const auto& position) { CreateImpactParticle(position); },
    [this]() { m_updateEvents.targetActivated = true; },
    [this]() { m_updateEvents.mineExploded = true; }
  };

  geometry_containment<mine> m_mineContainment { [this](auto& mine)
  {
    auto position = mine->PreviousPosition();
    CreateExplosion(position);
    mine->Destroy();
    m_updateEvents.mineExploded = true;
  }};

  geometry_containment<player_ship> m_shipContainment { [this](auto& ship)
  {
    auto position = ship->PreviousPosition();
    CreateExplosion(position);
    ship->ApplyFatalDamage();
  }};

  particle_containment<explosion_particle> m_explosionContainment { [this](auto& particle)
  {
    particle.Destroy();
  }};

  particle_containment<thrust_particle> m_thrustContainment { [this](auto& particle)
  {
    particle.Destroy();
  }};

  particle_containment<bullet> m_bulletContainment { [this](auto& bullet)
  {
    m_impactParticles.emplace_back(bullet.Position());
    bullet.Destroy();
  }};

};

inline auto level_container::update_events::reset() -> void
{
  playerShot = false ;
  targetActivated = false;
  mineExploded = false;
}

auto level_container::AddBlankObjects(std::ranges::input_range auto&& objects) -> void
{
  std::ranges::for_each(objects, [this](const auto& object)
  {
    m_blankObjects.push_back(object);
  });
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

inline [[nodiscard]] auto level_container::UpdateEvents() const -> const update_events&
{
  return m_updateEvents;
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
