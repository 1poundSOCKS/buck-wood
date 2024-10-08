#pragma once

#include "level_container.h"

struct create_new_objects_visitor
{
  level_container* m_levelContainer;
  std::shared_ptr<play_events> m_events;

  auto operator()(enemy_ship& object) -> void
  {
    if( !m_levelContainer->PlayerState().Destroyed() && object.CanShootAt(m_levelContainer->PlayerState().Position()) )
    {
      auto direction = direct2d::GetAngleBetweenPoints(object.Position(), m_levelContainer->PlayerState().Position());
      auto velocity = direct2d::CalculateVelocity(500.0f, direction);
      m_levelContainer->CreateEnemyBullet(enemy_bullet::type::two, object.Position(), velocity);
      m_events->SetEvent(play_events::event_type::shot, true);
    }
  }

  auto operator()(enemy_type_2& object) -> void
  {
    if( !m_levelContainer->PlayerState().Destroyed() && object.CanShootAt(m_levelContainer->PlayerState().Position()) )
    {
      auto direction = direct2d::GetAngleBetweenPoints(object.Position(), m_levelContainer->PlayerState().Position());
      auto velocity = direct2d::CalculateVelocity(500.0f, direction);
      m_levelContainer->CreateEnemyBullet(enemy_bullet::type::two, object.Position(), velocity);
      m_events->SetEvent(play_events::event_type::shot, true);
    }
  }

  auto operator()(player_ship& object) -> void
  {
    if( object.CanShoot() )
    {
      m_levelContainer->CreatePlayerBullet(object.Position(), direct2d::CalculateVelocity(1500, object.Angle()), 1);
      m_events->SetEvent(play_events::event_type::shot, true);
    }

    if( object.EmitThrustParticle() )
    {
      auto thrustAngle = direct2d::RotateAngle(object.Angle(), 180);
      auto thrustPosition = direct2d::CalculatePosition(object.Position(), thrustAngle, 20);
      auto thrustVelocity = direct2d::CombineVelocities(object.Velocity(), direct2d::CalculateVelocity(50.0f, thrustAngle));
      m_levelContainer->CreateParticle(particle::type::thrust, thrustPosition, thrustVelocity, 0.5f);
    }
  }

  auto operator()(auto&& object)
  {
  }

};
