#include "pch.h"
#include "level_container.h"
#include "renderers.h"
#include "game_settings.h"
#include "dynamic_object_functions.h"
#include "particle_functions.h"
#include "update_object_visitor.h"
#include "create_new_objects_visitor.h"
#include "save_player_state_visitor.h"
#include "level_collisions.h"

auto level_container::Update(float interval, D2D1_RECT_F viewRect) -> void
{
  auto updateStart = performance_counter::QueryValue();
  UpdateObjects(interval);

  auto collisionsStart = performance_counter::QueryValue();

  DoCollisions();

  auto collisionsEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"collisions", collisionsEnd - collisionsStart, game_settings::swapChainRefreshRate());

  save_player_state_visitor savePlayerStateVisitor { *this };
  for( const auto& object : m_movingObjects )
  {
    std::visit(savePlayerStateVisitor, object->Get());
  }

  RemoveDestroyedObjects();

  m_targettedObject = m_playerState.Destroyed() ? std::nullopt : GetTargettedObject();

  CreateNewObjects(interval);

  auto targetCounter = std::ranges::views::transform(m_staticObjects, [](const auto& object) { return std::holds_alternative<level_target>(object->Get()) ? 1 : 0; });
  m_targetsRemaining = std::accumulate(std::begin(targetCounter), std::end(targetCounter), 0);

  auto updateEnd = performance_counter::QueryValue();
  diagnostics::addTime(L"level_container::update", updateEnd - updateStart, game_settings::swapChainRefreshRate());
}

auto level_container::UpdateObjects(float interval) -> void
{
  dynamic_object_functions::update(m_particles, interval);

  update_object_visitor updateObjectVisitor { *this, interval };
  
  for( auto& object : m_staticObjects )
  {
    std::visit(updateObjectVisitor, object->Get());
    object.UpdateGeometry();
  }

  for( auto& object : m_movingObjects )
  {
    std::visit(updateObjectVisitor, object->Get());
    object.UpdateGeometry();
  }
}

auto level_container::RemoveDestroyedObjects() -> void
{
  particle_functions::erase_destroyed(m_particles);
  dynamic_object_functions::erase_destroyed(m_movingObjects);
  dynamic_object_functions::erase_destroyed(m_staticObjects);
}

auto level_container::Render(D2D1_RECT_F viewRect) const -> void
{
  auto renderStart = performance_counter::QueryValue();

  if( m_boundary.Geometry() )
  {
    renderer::render(m_boundary);
  }

  renderer::render_all(m_particles);
  renderer::reverse_render_all(m_movingObjects);
  renderer::render_all(m_staticObjects);

  auto renderEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"render", renderEnd - renderStart, game_settings::swapChainRefreshRate());
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

  // particle_collision<default_object, particle> particleCollisionRunner { collisionHandler };
  // particleCollisionRunner(m_staticObjects, m_particles);
  // particleCollisionRunner(m_movingObjects, m_particles);
}

auto level_container::CreateNewObjects(float interval) -> void
{
  for( const auto& position : m_explosions )
  {
    std::ranges::copy(level_explosion { position }, std::back_inserter(m_particles));
    m_playEvents->SetEvent(play_events::event_type::explosion, true);
  }

  m_explosions.clear();

  for( const auto& position : m_impacts )
  {
    m_particles.emplace_back(particle::type::impact, position, VELOCITY_2F { 0, 0 }, 0.5f);
  }

  m_impacts.clear();

  for( auto& object : m_staticObjects )
  {
    std::visit(create_new_objects_visitor { *this }, object.Object().Get());
  }

  for( auto& object : m_movingObjects )
  {
    std::visit(create_new_objects_visitor { *this }, object.Object().Get());
  }
}

auto level_container::CreateNewObjects(level_target& object) -> void
{
  if( !m_playerState.Destroyed() && object.CanShootAt(m_playerState.Position()) )
  {
    auto mineType = object.MineType();
    auto direction = direct2d::GetAngleBetweenPoints(object.Position(), m_playerState.Position());
    auto velocity = direct2d::CalculateVelocity(500.0f, direction);
    CreateMine(mineType, object.Position(), velocity);
    m_playEvents->SetEvent(play_events::event_type::shot, true);
  }
}

auto level_container::CreateNewObjects(player_ship& object) -> void
{
  if( object.CanShoot() )
  {
    auto targetPosition = TargettedObject() ? std::optional<POINT_2F>(TargettedObject()->Position()) : std::nullopt;
    auto bulletAngle = targetPosition ? direct2d::GetAngleBetweenPoints(object.Position(), *targetPosition) : object.Angle();
    CreatePlayerBullet(object.Position(), direct2d::CalculateVelocity(1500, bulletAngle), 1);
    m_playEvents->SetEvent(play_events::event_type::shot, true);
  }

  if( object.EmitThrustParticle() )
  {
    auto thrustAngle = direct2d::RotateAngle(object.Angle(), 180);
    auto thrustPosition = direct2d::CalculatePosition(object.Position(), thrustAngle, 20);
    auto thrustVelocity = direct2d::CombineVelocities(object.Velocity(), direct2d::CalculateVelocity(50.0f, thrustAngle));
    CreateParticle(particle::type::thrust, thrustPosition, thrustVelocity, 0.5f);
  }
}

auto level_container::GetTargettedObject() -> std::optional<targetted_object>
{
  auto targetAngle = m_playerState.Angle();

  constexpr auto angleSpan = 20.0f;

  auto mines = std::ranges::views::filter(m_movingObjects, [](const auto& object)
  {
    return object->HoldsAlternative<mine>();
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
