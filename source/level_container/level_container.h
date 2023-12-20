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
  auto DoBorderCollisions(const blank_object& border) -> void;
  auto CreateExplosion(const game_point& position) -> void;

private:

  reload_timer m_reloadTimer { static_cast<float>(1) / static_cast<float>(20) };
  reload_timer m_thrustEmmisionTimer { static_cast<float>(1) / static_cast<float>(10) };

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

  geometry_collision<player_ship, level_asteroid> m_shipToAsteroidCollision { [this](auto& playerShip, auto& asteroid)
  {
    auto position = playerShip.PreviousPosition();
    CreateExplosion(position);
    playerShip.ApplyFatalDamage();
  }};

  geometry_collision<player_ship, level_target> m_shipToTargetCollision { [this](auto& playerShip, auto& target)
  {
    playerShip.ApplyFatalDamage();
    auto position = playerShip.PreviousPosition();
    CreateExplosion(position);
  }};

  geometry_collision<player_ship, duct_fan> m_shipToDuctFanCollision { [this](auto& playerShip, auto& ductFan)
  {
    playerShip.ApplyFatalDamage();
    auto position = playerShip.PreviousPosition();
    CreateExplosion(position);
  }};

  geometry_collision<player_ship, mine> m_shipToMineCollision { [this](auto& playerShip, auto& mine)
  {
    playerShip.ApplyDamage(2);
    auto position = mine.PreviousPosition();
    CreateExplosion(position);
    mine.Destroy();
    m_updateEvents.mineExploded = true;
  }};

  geometry_collision<mine, level_asteroid> m_mineToAsteroidCollision { [this](auto& mine, auto& asteroid)
  {
    auto position = mine.PreviousPosition();
    CreateExplosion(position);
    mine.Destroy();
    m_updateEvents.mineExploded = true;
  }};

  geometry_collision<mine, duct_fan> m_mineToDuctFanCollision { [this](auto& mine, auto& ductFan)
  {
    auto position = mine.PreviousPosition();
    CreateExplosion(position);
    mine.Destroy();
    m_updateEvents.mineExploded = true;
  }};

  particle_collision<mine, bullet> m_mineToBulletCollision { [this](auto& mine, auto& bullet)
  {
    auto position = mine->PreviousPosition();
    CreateExplosion(position);
    mine->Destroy();
    bullet.Destroy();
    m_updateEvents.mineExploded = true;
  }};

  particle_collision<level_asteroid, bullet> m_asteroidToBulletCollision { [this](auto& asteroid, auto& bullet)
  {
    m_impactParticles.emplace_back(bullet.Position());
    bullet.Destroy();
  }};

  particle_collision<duct_fan, bullet> m_ductFanToBulletCollision { [this](auto& ductFan, auto& bullet)
  {
    m_impactParticles.emplace_back(bullet.Position());
    bullet.Destroy();
  }};

  particle_collision<level_target, bullet> m_targetToBulletCollision { [this](auto& target, auto& bullet)
  {
    m_impactParticles.emplace_back(bullet.Position());

    if( !target->IsActivated() )
    {
      target->HitByBullet();

      if( target->IsActivated() )
      {
        ++m_activatedTargetCount;
        m_updateEvents.targetActivated = true;
      }
    }

    bullet.Destroy();
  }};

  particle_collision<player_ship, explosion_particle> m_shipToExplosionCollision { [this](auto& playerShip, auto& particle)
  {
    particle.Destroy();
  }};

  particle_collision<level_asteroid, explosion_particle> m_asteroidToExplosionCollision { [this](auto& asteroid, auto& particle)
  {
    particle.Destroy();
  }};

  particle_collision<duct_fan, explosion_particle> m_ductFanToExplosionCollision { [this](auto& ductFan, auto& particle)
  {
    particle.Destroy();
  }};

  particle_collision<level_asteroid, thrust_particle> m_asteroidToThrustCollision { [this](auto& asteroid, auto& particle)
  {
    particle.Destroy();
  }};

  particle_collision<duct_fan, thrust_particle> m_ductFanToThrustCollision { [this](auto& ductFan, auto& particle)
  {
    particle.Destroy();
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

[[nodiscard]] inline auto level_container::PlayerAngle() const -> float
{
  return m_playerShip->Angle();
}

[[nodiscard]] inline auto level_container::PlayerShields() const -> const player_ship::shield_status&
{
  return m_playerShip->ShieldStatus();  
}

[[nodiscard]] inline auto level_container::UpdateEvents() const -> const update_events&
{
  return m_updateEvents;
}
