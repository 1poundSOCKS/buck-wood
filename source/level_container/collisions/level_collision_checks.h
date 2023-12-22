#pragma once

#include "dynamic_object_collection.h"
#include "particle_collection.h"

#include "geometry_collision.h"
#include "particle_collision.h"
#include "geometry_containment.h"
#include "particle_containment.h"

#include "level_objects.h"
#include "collision_objects.h"

class level_collision_checks
{

public:

  using explosion_collection = std::vector<game_point>;
  using impact_collection = std::vector<game_point>;

public:

  geometry_collision<player_ship, level_asteroid> shipToAsteroidCollision { [this](auto& playerShip, auto& asteroid)
  {
    playerShip.ApplyFatalDamage();
    m_explosions.emplace_back(playerShip.PreviousPosition());
  }};

  geometry_collision<player_ship, level_target> shipToTargetCollision { [this](auto& playerShip, auto& target)
  {
    playerShip.ApplyFatalDamage();
    m_explosions.emplace_back(playerShip.PreviousPosition());
  }};

  geometry_collision<player_ship, duct_fan> shipToDuctFanCollision { [this](auto& playerShip, auto& ductFan)
  {
    playerShip.ApplyFatalDamage();
    auto position = playerShip.PreviousPosition();
    m_explosions.emplace_back(playerShip.PreviousPosition());
  }};

  geometry_collision<player_ship, mine> shipToMineCollision { [this](auto& playerShip, auto& mine)
  {
    playerShip.ApplyDamage(2);
    mine.Destroy();
    m_explosions.emplace_back(mine.PreviousPosition());
  }};

  particle_collision<player_ship, explosion_particle> shipToExplosionCollision { [this](auto& playerShip, auto& particle)
  {
    particle.Destroy();
  }};

  geometry_collision<mine, level_asteroid> mineToAsteroidCollision { [this](auto& mine, auto& asteroid)
  {
    auto position = mine.PreviousPosition();
    mine.Destroy();
    m_explosions.emplace_back(mine.PreviousPosition());
  }};

  geometry_collision<mine, duct_fan> mineToDuctFanCollision { [this](auto& mine, auto& ductFan)
  {
    auto position = mine.PreviousPosition();
    mine.Destroy();
    m_explosions.emplace_back(mine.PreviousPosition());
  }};

  particle_collision<mine, bullet> mineToBulletCollision { [this](auto& mine, auto& bullet)
  {
    mine->Destroy();
    bullet.Destroy();
    m_explosions.emplace_back(mine->Position());
  }};

  particle_collision<level_asteroid, bullet> asteroidToBulletCollision { [this](auto& asteroid, auto& bullet)
  {
    bullet.Destroy();
    m_impacts.emplace_back(bullet.Position());
  }};

  particle_collision<duct_fan, bullet> ductFanToBulletCollision { [this](auto& ductFan, auto& bullet)
  {
    bullet.Destroy();
    m_impacts.emplace_back(bullet.Position());
  }};

  particle_collision<level_target, bullet> targetToBulletCollision { [this](auto& target, auto& bullet)
  {
    if( !target->IsActivated() )
    {
      target->HitByBullet();

      if( target->IsActivated() )
      {
        // m_targetActivated();
      }
    }

    bullet.Destroy();

    m_impacts.emplace_back(bullet.Position());
  }};

  particle_collision<level_asteroid, explosion_particle> asteroidToExplosionCollision { [this](auto& asteroid, auto& particle)
  {
    particle.Destroy();
  }};

  particle_collision<duct_fan, explosion_particle> ductFanToExplosionCollision { [this](auto& ductFan, auto& particle)
  {
    particle.Destroy();
  }};

  particle_collision<level_asteroid, thrust_particle> asteroidToThrustCollision { [this](auto& asteroid, auto& particle)
  {
    particle.Destroy();
  }};

  particle_collision<duct_fan, thrust_particle> ductFanToThrustCollision { [this](auto& ductFan, auto& particle)
  {
    particle.Destroy();
  }};

  [[nodiscard]] auto Explosions() const -> const explosion_collection&
  {
    return m_explosions;
  }

  [[nodiscard]] auto Impacts() const -> const impact_collection&
  {
    return m_impacts;
  }

  auto Reset() -> void
  {
    m_explosions.clear();
    m_impacts.clear();
  }

private:

  explosion_collection m_explosions;
  impact_collection m_impacts;

};
