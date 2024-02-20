#pragma once

#include "collisions/geometry_collision.h"
#include "collisions/particle_collision.h"
#include "collisions/geometry_containment.h"
#include "collisions/particle_containment.h"
#include "collisions/collision.h"

#include "level_data_types.h"

template <typename visitor_type>
class level_collision_handler
{

public:

  level_collision_handler(visitor_type& visitor) : m_visitor { visitor }
  {
  }

  auto operator()(default_object& object1, default_object& object2) -> void
  {
    object1.Destroy();
    object2.Destroy();

    if( object1.Destroyed() ) m_visitor.CreateExplosion(object1.Position());
    if( object2.Destroyed() ) m_visitor.CreateExplosion(object2.Position());
  }

  auto operator()(player_ship& ship, mine& mine) -> void
  {
    ship.ApplyFatalDamage();
    mine.Destroy();
    m_visitor.CreateExplosion(mine.PreviousPosition());
  }

  auto operator()(player_ship& ship, level_target& target) -> void
  {
    ship.ApplyFatalDamage();
    m_visitor.CreateExplosion(ship.PreviousPosition());
  }

  auto operator()(target_object& target, bullet& bullet) -> void
  {
    auto targetActivated = target->IsActivated();

    if( !targetActivated )
    {
#ifdef TARGETS_ARE_TARGETS
      target->HitByBullet();
      m_activatedTargetCount += target->IsActivated() ? 1 : 0;
#endif
    }

    if( !targetActivated && target->IsActivated() )
    {
      m_visitor.TargetActivated();
    }

    m_visitor.CreateImpact(bullet.Position());
    bullet.Destroy();
  }

  auto operator()(mine_object& mine, bullet& bullet) -> void
  {
    bullet.Destroy();

    if( mine->HardnessType() == mine::hardness_type::soft && bullet.DamageMode() == bullet::damage_mode::two || 
        mine->HardnessType() == mine::hardness_type::tough && bullet.DamageMode() == bullet::damage_mode::one )
    {
      m_visitor.CreateExplosion(mine->PreviousPosition());
      mine->Destroy();
      m_visitor.MineDestroyed();
    }
  }

  auto operator()(mine& mine1, mine& mine2) -> void
  {
    m_visitor.CreateExplosion(mine1.PreviousPosition());
    m_visitor.CreateExplosion(mine2.PreviousPosition());
    mine1.Destroy();
    mine2.Destroy();
  }

  template <typename particle_object_type> auto DestroyParticlesOnGeometryCollision(std::ranges::input_range auto&& particles, ID2D1Geometry* boundary, std::ranges::input_range auto&& asteroids, std::ranges::input_range auto&& ductFans) -> void;
  template <typename geometry_object_type> auto DestroyObjectsOnGeometryCollision(std::ranges::input_range auto&& objects, ID2D1Geometry* boundary, std::ranges::input_range auto&& asteroids, std::ranges::input_range auto&& ductFans) -> void;
  template <typename geometry_object_type> auto DestroyObjectOnGeometryCollision(auto& object, ID2D1Geometry* boundary, std::ranges::input_range auto&& asteroids, std::ranges::input_range auto&& ductFans) -> void;

  auto DestroyBulletsOnGeometryCollision(std::ranges::input_range auto&& bullets, ID2D1Geometry* boundary, std::ranges::input_range auto&& asteroids, std::ranges::input_range auto&& ductFans) -> void;

private:

  visitor_type& m_visitor;

};

template <typename visitor_type>
template <typename particle_object_type> auto level_collision_handler<visitor_type>::DestroyParticlesOnGeometryCollision(std::ranges::input_range auto&& particles, ID2D1Geometry* boundary, std::ranges::input_range auto&& asteroids, std::ranges::input_range auto&& ductFans) -> void
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

  destroyParticlesAtBoundary(boundary, particles);
  destroyParticlesOnAsteroids(asteroids, particles);
  destroyParticlesOnDuctFans(ductFans, particles);
}

template <typename visitor_type>
template <typename geometry_object_type> auto level_collision_handler<visitor_type>::DestroyObjectsOnGeometryCollision(std::ranges::input_range auto&& objects, ID2D1Geometry* boundary, std::ranges::input_range auto&& asteroids, std::ranges::input_range auto&& ductFans) -> void
{
  geometry_containment<geometry_object_type> destroyObjectsAtBoundary { [this](auto& object)
  {
    m_visitor.CreateExplosion(object->PreviousPosition());
    object->Destroy();
  }};

  geometry_collision<level_asteroid, geometry_object_type> destroyObjectsOnAsteroids { [this](auto& asteroid, auto& object)
  {
    m_visitor.CreateExplosion(object.PreviousPosition());
    object.Destroy();
  }};

  geometry_collision<duct_fan, geometry_object_type> destroyObjectsOnDuctFans { [this](auto& ductFan, auto& object)
  {
    m_visitor.CreateExplosion(object.PreviousPosition());
    object.Destroy();
  }};

  destroyObjectsAtBoundary(boundary, objects);
  destroyObjectsOnAsteroids(asteroids, objects);
  destroyObjectsOnDuctFans(ductFans, objects);
}

template <typename visitor_type>
template <typename geometry_object_type> auto level_collision_handler<visitor_type>::DestroyObjectOnGeometryCollision(auto& object, ID2D1Geometry* boundary, std::ranges::input_range auto&& asteroids, std::ranges::input_range auto&& ductFans) -> void
{
  geometry_containment<geometry_object_type> destroyObjectAtBoundary { [this](auto& object)
  {
    m_visitor.CreateExplosion(object->PreviousPosition());
    object->Destroy();
  }};

  geometry_collision<geometry_object_type, level_asteroid> destroyObjectOnAsteroids { [this](auto& object, auto& asteroid)
  {
    m_visitor.CreateExplosion(object.PreviousPosition());
    object.Destroy();
  }};

  geometry_collision<geometry_object_type, duct_fan> destroyObjectOnDuctFans { [this](auto& object, auto& ductFan)
  {
    m_visitor.CreateExplosion(object.PreviousPosition());
    object.Destroy();
  }};

  destroyObjectAtBoundary(boundary, object);
  destroyObjectOnAsteroids(object, asteroids);
  destroyObjectOnDuctFans(object, ductFans);
}

template <typename visitor_type>
auto level_collision_handler<visitor_type>::DestroyBulletsOnGeometryCollision(std::ranges::input_range auto&& bullets, ID2D1Geometry* boundary, std::ranges::input_range auto&& asteroids, std::ranges::input_range auto&& ductFans) -> void
{
  auto unaryFunction = [this](auto& particle)
  {
    m_visitor.CreateImpact(particle.Position());
    particle.Destroy();
  };

  auto binaryFunction =  [this](auto& geometry, auto& particle)
  {
    m_visitor.CreateImpact(particle.Position());
    particle.Destroy();
  };

  particle_containment<bullet> destroyBulletsAtBoundary { unaryFunction };
  destroyBulletsAtBoundary(boundary, bullets);

  particle_collision<level_asteroid, bullet> destroyBulletsOnAsteroids { binaryFunction };
  destroyBulletsOnAsteroids(asteroids, bullets);

  particle_collision<duct_fan, bullet> destroyBulletsOnDuctFans { binaryFunction };
  destroyBulletsOnDuctFans(ductFans, bullets);
}
