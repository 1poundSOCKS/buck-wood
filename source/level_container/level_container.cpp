#include "pch.h"
#include "level_container.h"
#include "renderers.h"
#include "particle_functions.h"
#include "update_object_visitor.h"
#include "create_new_objects_visitor.h"
#include "save_player_state_visitor.h"
#include "level_collisions.h"

auto level_container::RemoveDestroyedObjects() -> void
{
  particle_functions::erase_destroyed(m_particles);
  dynamic_object_functions::erase_destroyed(m_movingObjects);
  dynamic_object_functions::erase_destroyed(m_staticObjects);
}

auto level_container::DoCollisions() -> void
{
  level_collision_handler collisionHandler { *this };
  
  if( m_boundary.Geometry() )
  {
    geometry_containment<default_object> geometryContainmentRunner { collisionHandler };
    geometryContainmentRunner(m_boundary.Geometry().get(), m_movingObjects);

    particle_containment<particle> particleContainmentRunner { collisionHandler };
    particleContainmentRunner(m_boundary.Geometry().get(), m_particles);
  }

  geometry_collision_binary<default_object, default_object> staticMovingCollisionRunner { collisionHandler };
  staticMovingCollisionRunner(m_staticObjects, m_movingObjects);

  geometry_collision_unary<default_object> movingCollisionRunner { collisionHandler };
  movingCollisionRunner(m_movingObjects);

#ifdef ALL_PARTICLE_COLLISIONS_TESTED
  particle_collision<default_object, particle> particleCollisionRunner { collisionHandler };
  particleCollisionRunner(m_staticObjects, m_particles);
  particleCollisionRunner(m_movingObjects, m_particles);
#endif
}

auto level_container::GetTargettedObject() -> std::optional<targetted_object>
{
  auto targetAngle = m_playerState.Angle();

  constexpr auto angleSpan = 20.0f;

  auto mines = std::ranges::views::filter(m_movingObjects, [](const auto& object)
  {
    return object->HoldsAlternative<enemy_bullet_1>();
  });

  dynamic_object<default_object>* nearestObject = std::accumulate(std::begin(mines), std::end(mines), 
  static_cast<dynamic_object<default_object>*>(nullptr), 
  [this, targetAngle](auto* nearest, auto& next) -> dynamic_object<default_object>*
  {
    auto mineAngle = direct2d::GetAngleBetweenPoints(m_playerState.Position(), next->Position());
    auto angleDifference = direct2d::GetAngleDifference(targetAngle, mineAngle);
    if( angleDifference < -angleSpan || angleDifference > angleSpan ) return nearest;
    else return nearest ? &GetNearestToTarget(*nearest, next) : &next;
  });

  return nearestObject ? std::optional<targetted_object>(nearestObject) : std::nullopt;
}
