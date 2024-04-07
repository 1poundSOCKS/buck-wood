#include "pch.h"
#include "level_container.h"
#include "renderers.h"
#include "particle_functions.h"
#include "update_object_visitor.h"
#include "level_collision_handler.h"

auto level_container::Update(float interval, D2D1_RECT_F viewRect) -> void
{
  update_object_visitor updateObjectVisitor { this, interval };
  level_collision_handler collisionHandler { this };
  
  Update(updateObjectVisitor, collisionHandler, viewRect);
}

auto level_container::RemoveDestroyedObjects() -> void
{
  particle_functions::erase_destroyed(m_particles);
  dynamic_object_functions::erase_destroyed(m_enemyObjects);
  dynamic_object_functions::erase_destroyed(m_playerObjects);
}

auto level_container::GetTargettedObject() -> std::optional<targetted_object>
{
  constexpr auto angleSpan = 40.0f;

  auto targetableObjects = std::ranges::views::filter(m_enemyObjects, [](const auto& object)
  {
    return object->HoldsAlternative<enemy_type_1>();
  });

  dynamic_object<default_object>* nearestObject = std::accumulate(std::begin(targetableObjects), std::end(targetableObjects), 
  static_cast<dynamic_object<default_object>*>(nullptr), 
  [this](auto* nearest, auto& next) -> dynamic_object<default_object>*
  {
    auto targetAngle = direct2d::GetAngleBetweenPoints(m_playerState.Position(), next->Position());
    auto angleDifference = direct2d::GetAngleDifference(m_playerState.Angle(), targetAngle);
    if( angleDifference < -angleSpan || angleDifference > angleSpan ) return nearest;
    else return nearest ? &GetNearestToTarget(*nearest, next) : &next;
  });

  return nearestObject ? std::optional<targetted_object>(nearestObject) : std::nullopt;
}

auto level_container::UpdateObject(player_ship& object, float interval) -> void
{
  object.Update(interval, m_playerActive);

  if( object.CanShoot() )
  {
    CreatePlayerBullet(object.Position(), direct2d::CalculateVelocity(1500, object.Angle()), 1);
    play_events::set(play_events::event_type::shot, true);
  }

  if( object.EmitThrustParticle() )
  {
    auto thrustAngle = direct2d::RotateAngle(object.Angle(), 180);
    auto thrustPosition = direct2d::CalculatePosition(object.Position(), thrustAngle, 20);
    auto thrustVelocity = direct2d::CombineVelocities(object.Velocity(), direct2d::CalculateVelocity(50.0f, thrustAngle));
    CreateParticle(particle::type::thrust, thrustPosition, thrustVelocity, 0.5f);
  }
}

auto level_container::UpdateObject(enemy_type_1& object, float interval) -> void
{
  object.Update(interval, m_playerState.Position());

  if( !m_playerState.Destroyed() && object.CanShootAt(m_playerState.Position()) )
  {
    auto direction = direct2d::GetAngleBetweenPoints(object.Position(), m_playerState.Position());
    auto velocity = direct2d::CalculateVelocity(500.0f, direction);
    CreateEnemyBullet(enemy_bullet_1::type::two, object.Position(), velocity);
    play_events::set(play_events::event_type::shot, true);
  }
}

auto level_container::UpdateObject(enemy_type_2& object, float interval) -> void
{
  object.Update(interval);

  if( !m_playerState.Destroyed() && object.CanShootAt(m_playerState.Position()) )
  {
    auto direction = direct2d::GetAngleBetweenPoints(object.Position(), m_playerState.Position());
    auto velocity = direct2d::CalculateVelocity(500.0f, direction);
    CreateEnemyBullet(enemy_bullet_1::type::two, object.Position(), velocity);
    play_events::set(play_events::event_type::shot, true);
  }
}
