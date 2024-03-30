#include "pch.h"
#include "level_container.h"
#include "renderers.h"
#include "particle_functions.h"
#include "update_object_visitor.h"
#include "create_new_objects_visitor.h"
#include "save_player_state_visitor.h"
#include "level_collision_handler.h"

auto level_container::Update(float interval, D2D1_RECT_F viewRect) -> void
{
  update_object_visitor m_updateObjectVisitor { this, interval, PlayerActive() };
  create_new_objects_visitor createNewObjectsVisitor { this, m_playEvents };
  save_player_state_visitor savePlayerStateVisitor { this };
  level_collision_handler collisionHandler { this, m_playEvents };
  
  Update(m_updateObjectVisitor, savePlayerStateVisitor, createNewObjectsVisitor, collisionHandler, viewRect);
}

auto level_container::RemoveDestroyedObjects() -> void
{
  particle_functions::erase_destroyed(m_particles);
  dynamic_object_functions::erase_destroyed(m_movingObjects);
  dynamic_object_functions::erase_destroyed(m_staticObjects);
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

auto level_container::UpdateObject(enemy_type_1& object, float interval) -> void
{
  object.Update(interval);
}

auto level_container::UpdateObject(enemy_type_2& object, float interval) -> void
{
  object.Update(interval);
  auto [position, destination] = m_enemyMovementRandom(object.Position(), object.Destination(), interval);
  object.SetPosition(position);
  object.SetDestination(destination);
}
