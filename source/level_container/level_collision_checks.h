#pragma once

#include "dynamic_object_collection.h"
#include "particle_collection.h"

#include "geometry_collision.h"
#include "particle_collision.h"
#include "geometry_containment.h"
#include "particle_containment.h"

#include "player_ship.h"
#include "level_target.h"
#include "mine.h"
#include "level_asteroid.h"
#include "duct_fan.h"
#include "bullet.h"
#include "impact_particle.h"
#include "explosion_particle.h"
#include "thrust_particle.h"

class level_collision_checks
{

public:

  level_collision_checks(auto&& createExplosion, auto&& createImpactParticle, auto&& targetActivated, auto&& mineExploded) : 
    m_createExplosion { createExplosion }, m_createImpactParticle { createImpactParticle }, m_targetActivated { targetActivated }, m_mineExploded { mineExploded }
  {
  }

  auto operator()(dynamic_object<player_ship>& ship, dynamic_object_collection<level_asteroid>& asteroids) -> void
  {
    m_shipToAsteroidCollision(ship, asteroids);
  }

  auto operator()(dynamic_object<player_ship>& ship, dynamic_object_collection<level_target>& targets) -> void
  {
    m_shipToTargetCollision(ship, targets);
  }

  auto operator()(dynamic_object<player_ship>& ship, dynamic_object_collection<duct_fan>& ductFans) -> void
  {
    m_shipToDuctFanCollision(ship, ductFans);
  }

  auto operator()(dynamic_object<player_ship>& ship, dynamic_object_collection<mine>& mines) -> void
  {
    m_shipToMineCollision(ship, mines);
  }

  auto operator()(dynamic_object<player_ship>& ship, particle_collection<explosion_particle>& particles) -> void
  {
    m_shipToExplosionCollision(ship, particles);
  }

  auto operator()(dynamic_object_collection<mine>& mines, dynamic_object_collection<level_asteroid>& asteroids) -> void
  {
    m_mineToAsteroidCollision(mines, asteroids);
  }

  auto operator()(dynamic_object_collection<mine>& mines, dynamic_object_collection<duct_fan>& ductFans) -> void
  {
    m_mineToDuctFanCollision(mines, ductFans);
  }

  auto operator()(dynamic_object_collection<mine>& mines, particle_collection<bullet>& bullets) -> void
  {
    m_mineToBulletCollision(mines, bullets);
  }

  auto operator()(dynamic_object_collection<level_asteroid>& asteroids, particle_collection<bullet>& bullets) -> void
  {
    m_asteroidToBulletCollision(asteroids, bullets);
  }

  auto operator()(dynamic_object_collection<duct_fan>& ductFans, particle_collection<bullet>& bullets) -> void
  {
    m_ductFanToBulletCollision(ductFans, bullets);
  }

  auto operator()(dynamic_object_collection<level_target>& targets, particle_collection<bullet>& bullets) -> void
  {
    m_targetToBulletCollision(targets, bullets);
  }

  auto operator()(dynamic_object_collection<level_asteroid>& asteroids, particle_collection<explosion_particle>& particles) -> void
  {
    m_asteroidToExplosionCollision(asteroids, particles);
  }

  auto operator()(dynamic_object_collection<duct_fan>& ductFans, particle_collection<explosion_particle>& particles) -> void
  {
    m_ductFanToExplosionCollision(ductFans, particles);
  }

  auto operator()(dynamic_object_collection<level_asteroid>& asteroids, particle_collection<thrust_particle>& particles) -> void
  {
    m_asteroidToThrustCollision(asteroids, particles);
  }

  auto operator()(dynamic_object_collection<duct_fan>& ductFans, particle_collection<thrust_particle>& particles) -> void
  {
    m_ductFanToThrustCollision(ductFans, particles);
  }

private:

  std::function<void(const game_point&)> m_createExplosion;
  std::function<void(const game_point&)> m_createImpactParticle;
  std::function<void()> m_targetActivated;
  std::function<void()> m_mineExploded;

  geometry_collision<player_ship, level_asteroid> m_shipToAsteroidCollision { [this](auto& playerShip, auto& asteroid)
  {
    auto position = playerShip.PreviousPosition();
    m_createExplosion(position);
    playerShip.ApplyFatalDamage();
  }};

  geometry_collision<player_ship, level_target> m_shipToTargetCollision { [this](auto& playerShip, auto& target)
  {
    playerShip.ApplyFatalDamage();
    auto position = playerShip.PreviousPosition();
    m_createExplosion(position);
  }};

  geometry_collision<player_ship, duct_fan> m_shipToDuctFanCollision { [this](auto& playerShip, auto& ductFan)
  {
    playerShip.ApplyFatalDamage();
    auto position = playerShip.PreviousPosition();
    m_createExplosion(position);
  }};

  geometry_collision<player_ship, mine> m_shipToMineCollision { [this](auto& playerShip, auto& mine)
  {
    playerShip.ApplyDamage(2);
    auto position = mine.PreviousPosition();
    m_createExplosion(position);
    mine.Destroy();
    m_mineExploded();
  }};

  particle_collision<player_ship, explosion_particle> m_shipToExplosionCollision { [this](auto& playerShip, auto& particle)
  {
    particle.Destroy();
  }};

  geometry_collision<mine, level_asteroid> m_mineToAsteroidCollision { [this](auto& mine, auto& asteroid)
  {
    auto position = mine.PreviousPosition();
    m_createExplosion(position);
    mine.Destroy();
    m_mineExploded();
  }};

  geometry_collision<mine, duct_fan> m_mineToDuctFanCollision { [this](auto& mine, auto& ductFan)
  {
    auto position = mine.PreviousPosition();
    m_createExplosion(position);
    mine.Destroy();
    m_mineExploded();
  }};

  particle_collision<mine, bullet> m_mineToBulletCollision { [this](auto& mine, auto& bullet)
  {
    auto position = mine->PreviousPosition();
    m_createExplosion(position);
    mine->Destroy();
    bullet.Destroy();
    m_mineExploded();
  }};

  particle_collision<level_asteroid, bullet> m_asteroidToBulletCollision { [this](auto& asteroid, auto& bullet)
  {
    m_createImpactParticle(bullet.Position());
    bullet.Destroy();
  }};

  particle_collision<duct_fan, bullet> m_ductFanToBulletCollision { [this](auto& ductFan, auto& bullet)
  {
    m_createImpactParticle(bullet.Position());
    bullet.Destroy();
  }};

  particle_collision<level_target, bullet> m_targetToBulletCollision { [this](auto& target, auto& bullet)
  {
    m_createImpactParticle(bullet.Position());

    if( !target->IsActivated() )
    {
      target->HitByBullet();

      if( target->IsActivated() )
      {
        m_targetActivated();
      }
    }

    bullet.Destroy();
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

  geometry_containment<mine> m_mineContainment { [this](auto& mine)
  {
    auto position = mine->PreviousPosition();
    m_createExplosion(position);
    mine->Destroy();
    m_mineExploded();
  }};

  geometry_containment<player_ship> m_shipContainment { [this](auto& ship)
  {
    auto position = ship->PreviousPosition();
    m_createExplosion(position);
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
    m_createImpactParticle(bullet.Position());
    bullet.Destroy();
  }};

};
