#pragma once

#include "geometry_collision.h"
#include "level_objects.h"

namespace collision_object
{

class ship_to_asteroid : public geometry_collision<player_ship, level_asteroid>
{
public:
  
  ship_to_asteroid() :
    geometry_collision
    {
      [this](auto& playerShip, auto& asteroid)
      {
        auto position = playerShip.PreviousPosition();
        // m_createExplosion(position);
        playerShip.ApplyFatalDamage();
      }
    }
  {
  }

};

}

// geometry_collision<player_ship, level_target> m_shipToTargetCollision { [this](auto& playerShip, auto& target)
// {
//   playerShip.ApplyFatalDamage();
//   auto position = playerShip.PreviousPosition();
//   m_createExplosion(position);
// }};

// geometry_collision<player_ship, duct_fan> m_shipToDuctFanCollision { [this](auto& playerShip, auto& ductFan)
// {
//   playerShip.ApplyFatalDamage();
//   auto position = playerShip.PreviousPosition();
//   m_createExplosion(position);
// }};

// geometry_collision<player_ship, mine> m_shipToMineCollision { [this](auto& playerShip, auto& mine)
// {
//   playerShip.ApplyDamage(2);
//   auto position = mine.PreviousPosition();
//   m_createExplosion(position);
//   mine.Destroy();
//   m_mineExploded();
// }};

// particle_collision<player_ship, explosion_particle> m_shipToExplosionCollision { [this](auto& playerShip, auto& particle)
// {
//   particle.Destroy();
// }};

// geometry_collision<mine, level_asteroid> m_mineToAsteroidCollision { [this](auto& mine, auto& asteroid)
// {
//   auto position = mine.PreviousPosition();
//   m_createExplosion(position);
//   mine.Destroy();
//   m_mineExploded();
// }};

// geometry_collision<mine, duct_fan> m_mineToDuctFanCollision { [this](auto& mine, auto& ductFan)
// {
//   auto position = mine.PreviousPosition();
//   m_createExplosion(position);
//   mine.Destroy();
//   m_mineExploded();
// }};

// particle_collision<mine, bullet> m_mineToBulletCollision { [this](auto& mine, auto& bullet)
// {
//   auto position = mine->PreviousPosition();
//   m_createExplosion(position);
//   mine->Destroy();
//   bullet.Destroy();
//   m_mineExploded();
// }};

// particle_collision<level_asteroid, bullet> m_asteroidToBulletCollision { [this](auto& asteroid, auto& bullet)
// {
//   m_createImpactParticle(bullet.Position());
//   bullet.Destroy();
// }};

// particle_collision<duct_fan, bullet> m_ductFanToBulletCollision { [this](auto& ductFan, auto& bullet)
// {
//   m_createImpactParticle(bullet.Position());
//   bullet.Destroy();
// }};

// particle_collision<level_target, bullet> m_targetToBulletCollision { [this](auto& target, auto& bullet)
// {
//   m_createImpactParticle(bullet.Position());

//   if( !target->IsActivated() )
//   {
//     target->HitByBullet();

//     if( target->IsActivated() )
//     {
//       m_targetActivated();
//     }
//   }

//   bullet.Destroy();
// }};

// particle_collision<level_asteroid, explosion_particle> m_asteroidToExplosionCollision { [this](auto& asteroid, auto& particle)
// {
//   particle.Destroy();
// }};

// particle_collision<duct_fan, explosion_particle> m_ductFanToExplosionCollision { [this](auto& ductFan, auto& particle)
// {
//   particle.Destroy();
// }};

// particle_collision<level_asteroid, thrust_particle> m_asteroidToThrustCollision { [this](auto& asteroid, auto& particle)
// {
//   particle.Destroy();
// }};

// particle_collision<duct_fan, thrust_particle> m_ductFanToThrustCollision { [this](auto& ductFan, auto& particle)
// {
//   particle.Destroy();
// }};

// geometry_containment<mine> m_mineContainment { [this](auto& mine)
// {
//   auto position = mine->PreviousPosition();
//   m_createExplosion(position);
//   mine->Destroy();
//   m_mineExploded();
// }};

// geometry_containment<player_ship> m_shipContainment { [this](auto& ship)
// {
//   auto position = ship->PreviousPosition();
//   m_createExplosion(position);
//   ship->ApplyFatalDamage();
// }};

// particle_containment<explosion_particle> m_explosionContainment { [this](auto& particle)
// {
//   particle.Destroy();
// }};

// particle_containment<thrust_particle> m_thrustContainment { [this](auto& particle)
// {
//   particle.Destroy();
// }};

// particle_containment<bullet> m_bulletContainment { [this](auto& bullet)
// {
//   m_createImpactParticle(bullet.Position());
//   bullet.Destroy();
// }};
