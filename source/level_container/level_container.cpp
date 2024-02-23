#include "pch.h"
#include "level_container.h"
#include "renderers.h"
#include "game_settings.h"
#include "dynamic_object_functions.h"
#include "particle_functions.h"

struct update_objects_visitor
{
  level_container& m_levelContainer;
  dynamic_object<default_object>& m_dynamicObject;
  float m_interval;

  auto operator()(const level_target& object)
  {
    m_dynamicObject.Update(level_geometries::TargetGeometry(), m_interval, m_levelContainer.PlayerPosition());
  }

  auto operator()(const player_ship& object)
  {
    m_dynamicObject.Update(level_geometries::PlayerShipGeometry(), m_interval, m_levelContainer.PlayerPosition());
  }

  auto operator()(const mine& object)
  {
    m_dynamicObject.Update(level_geometries::MineGeometry(), m_interval, m_levelContainer.PlayerPosition());
  }

  auto operator()(const auto& object)
  {
  }
};

struct create_new_objects_visitor
{
  level_container& m_levelContainer;

  auto operator()(level_target& object)
  {
    if( m_levelContainer.PlayerPosition() && object.CanShootAt(*m_levelContainer.PlayerPosition()) )
    {
      auto [thrust, maxSpeed, type] = m_levelContainer.MineParameters();
      m_levelContainer.CreateMovingObject(level_geometries::MineGeometry(), std::in_place_type<mine>, object.Position(), thrust, maxSpeed, type);
    }
  }

  auto operator()(player_ship& object)
  {
    if( object.CanShoot() )
    {
      auto targetPosition = m_levelContainer.TargettedObject() ? std::optional<POINT_2F>(m_levelContainer.TargettedObject()->Position()) : std::nullopt;
      auto bulletAngle = targetPosition ? direct2d::GetAngleBetweenPoints(object.Position(), *targetPosition) : object.Angle();
      auto particleType = level_container::ConvertFireModeToParticleType(object.FireMode());
      m_levelContainer.CreateParticle(particleType, object.Position(), direct2d::CalculateVelocity(500, bulletAngle), 1.0f);
      m_levelContainer.SetPlayEvent(play_events::event_type::shot, true);
    }

    if( object.EmitThrustParticle() )
    {
      auto thrustAngle = direct2d::RotateAngle(object.Angle(), 180);
      auto thrustPosition = direct2d::CalculatePosition(object.Position(), thrustAngle, 20);
      auto thrustVelocity = direct2d::CombineVelocities(object.Velocity(), direct2d::CalculateVelocity(50.0f, thrustAngle));
      m_levelContainer.CreateParticle(particle::type::thrust, thrustPosition, thrustVelocity, 0.5f);
    }
  }

  auto operator()(auto& object)
  {
  }
};

auto level_container::SetPlayerActive(bool value) -> void
{
  m_playerActive = value;
}

auto level_container::Update(float interval, D2D1_RECT_F viewRect) -> void
{
  auto updateStart = performance_counter::QueryValue();
  UpdateObjects(interval);

  auto collisionsStart = performance_counter::QueryValue();

  DoCollisions();

  auto collisionsEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"collisions", collisionsEnd - collisionsStart, game_settings::swapChainRefreshRate());

  RemoveDestroyedObjects();

  m_targettedObject = GetTargettedObject();

  CreateNewObjects(interval);

  auto updateEnd = performance_counter::QueryValue();
  diagnostics::addTime(L"level_container::update", updateEnd - updateStart, game_settings::swapChainRefreshRate());
}

auto level_container::UpdateObjects(float interval) -> void
{
  dynamic_object_functions::update(m_particles, interval);

  for( auto& object : m_staticObjects )
  {
    std::visit(update_objects_visitor { *this, object, interval }, object->Get());
  }

  for( auto& object : m_movingObjects )
  {
    std::visit(update_objects_visitor { *this, object, interval }, object->Get());
  }
}

auto level_container::RemoveDestroyedObjects() -> void
{
  particle_functions::erase_destroyed(m_particles);
  dynamic_object_functions::erase_destroyed(m_movingObjects);
}

auto level_container::Render(D2D1_RECT_F viewRect) const -> void
{
  auto renderStart = performance_counter::QueryValue();

  renderer::render(m_boundary);
  renderer::render_all(m_staticObjects);
  renderer::render_all(m_movingObjects);
  renderer::render_all(m_particles);

  auto renderEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"render", renderEnd - renderStart, game_settings::swapChainRefreshRate());
}

auto level_container::DoCollisions() -> void
{
  level_collision_handler<level_container> collisionHandler { *this };
  
  geometry_containment<default_object> geometryContainmentRunner { collisionHandler };
  geometryContainmentRunner(m_boundary.Geometry().get(), m_movingObjects);

  particle_containment<particle> particleContainmentRunner { collisionHandler };
  particleContainmentRunner(m_boundary.Geometry().get(), m_particles);

  geometry_collision_binary<default_object, default_object> staticMovingCollisionRunner { collisionHandler };
  staticMovingCollisionRunner(m_staticObjects, m_movingObjects);

  geometry_collision_unary<default_object> movingCollisionRunner { collisionHandler };
  movingCollisionRunner(m_movingObjects);

  particle_collision<default_object, particle> particleCollisionRunner { collisionHandler };
  particleCollisionRunner(m_staticObjects, m_particles);
  particleCollisionRunner(m_movingObjects, m_particles);
}

auto level_container::CreateNewObjects(float interval) -> void
{
  for( const auto& position : m_explosions )
  {
    std::ranges::copy(level_explosion { position }, std::back_inserter(m_particles));
    m_playEvents.SetEvent(play_events::event_type::explosion, true);
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

auto level_container::GetTargettedObject() -> std::optional<targetted_object>
{
  m_targetPosition = direct2d::CalculatePosition(m_playerState->m_position, m_playerState->m_angle, 100);

  if( m_targetPosition )
  {
    auto targetAngle = m_playerState->m_angle;

    constexpr auto angleSpan = 20.0f;

    dynamic_object<default_object>* nearestObject = std::accumulate(std::begin(m_movingObjects), std::end(m_movingObjects), 
    static_cast<dynamic_object<default_object>*>(nullptr), 
    [this, targetAngle](auto* nearest, auto& next) -> dynamic_object<default_object>*
    {
      auto mineAngle = direct2d::GetAngleBetweenPoints(m_playerState->m_position, next->Position());
      auto angleDifference = direct2d::GetAngleDifference(targetAngle, mineAngle);
      if( angleDifference < -angleSpan || angleDifference > angleSpan ) return nearest;
      else return nearest ? &GetNearestToTarget(*nearest, next) : &next;
    });

    return nearestObject ? std::optional<targetted_object>(nearestObject) : std::nullopt;
  }
  else
  {
    return std::nullopt;
  }
}
