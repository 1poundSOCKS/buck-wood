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

  auto operator()(player_ship& ship, mine& mine) -> void
  {
    if( ship.ShieldsUp() && mine.HardnessType() == mine::hardness_type::soft || !ship.ShieldsUp() && mine.HardnessType() == mine::hardness_type::tough )
    {      
      ship.ApplyFatalDamage();
    }

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

    if( mine->HardnessType() == mine::hardness_type::soft )
    {
      m_visitor.CreateExplosion(mine->PreviousPosition());
      mine->Destroy();
    }
  }

  auto operator()(mine& mine1, mine& mine2) -> void
  {
    m_visitor.CreateExplosion(mine1.PreviousPosition());
    m_visitor.CreateExplosion(mine2.PreviousPosition());
    mine1.Destroy();
    mine2.Destroy();
  }

private:

  visitor_type& m_visitor;

};
