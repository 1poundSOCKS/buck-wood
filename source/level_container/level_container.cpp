#include "pch.h"
#include "level_container.h"
#include "renderers.h"
#include "particle_functions.h"
#include "update_object_visitor.h"
#include "level_collision_handler.h"
#include "player_state.h"

auto level_container::AddFloorCell(int x, int y, level_cell_type cellType) -> void
{
  m_cells.Add(x, y, cellType);
}

auto level_container::AddWalls() -> void
{
  m_cells.AddWalls();
}

auto level_container::Update(float interval, D2D1_RECT_F viewRect) -> void
{
  UpdateNew(interval, viewRect);
}

auto level_container::UpdateNew(float interval, D2D1_RECT_F viewRect) -> void
{
  auto collisionsStart = performance_counter::QueryValue();
  DoCollisions();
  auto collisionsEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"collisions", collisionsEnd - collisionsStart, game_settings::swapChainRefreshRate());

  auto updateStart = performance_counter::QueryValue();
  UpdateObjectsNew(interval);

  for( const auto& object : m_playerObjects )
  {
    auto ship = object->GetIf<player_ship>();
    m_playerState = ship ? *ship : m_playerState;
  }

  RemoveDestroyedObjects();

  // m_targettedObject = m_playerState.TargettingActive() ? GetTargettedObject() : std::nullopt;

  auto enemies = std::ranges::views::transform(m_enemyObjects, [](const auto& object)
  {
    return std::holds_alternative<enemy_type_1>(object->Get()) || std::holds_alternative<enemy_type_2>(object->Get()) ? 1 : 0;
  });

  m_enemyCount = m_enemyObjects.size();//std::accumulate(std::begin(enemies), std::end(enemies), 0);

  auto updateEnd = performance_counter::QueryValue();
  diagnostics::addTime(L"level_container::update", updateEnd - updateStart, game_settings::swapChainRefreshRate());
}

auto level_container::UpdateObjectsNew(float interval) -> void
{
  auto particles = std::ranges::views::filter(m_particles, [](const auto& particle) { return !particle.Destroyed(); } );

  dynamic_object_functions::update(particles, interval);

  auto NoninteractiveObjects = std::ranges::views::filter(m_noninteractiveObjects, [](const auto& object) { return !object->Destroyed(); } );

  for( auto& object : NoninteractiveObjects )
  {
    std::visit([this, interval](auto& object){ UpdateObject(object, interval); }, object->Get());
    object.UpdateGeometry();
  }

  auto PlayerObjects = std::ranges::views::filter(m_playerObjects, [](const auto& object) { return !object->Destroyed(); } );

  for( auto& object : PlayerObjects )
  {
    std::visit([this, interval](auto& object){ UpdateObject(object, interval); }, object->Get());
    object.UpdateGeometry();
  }

  auto EnemyObjects = std::ranges::views::filter(m_enemyObjects, [](const auto& object) { return !object->Destroyed(); } );

  for( auto& object : EnemyObjects )
  {
    std::visit([this, interval](auto& object){ UpdateObject(object, interval); }, object->Get());
    object.UpdateGeometry();
  }
}

auto level_container::RemoveDestroyedObjects() -> void
{
  particle_functions::erase_destroyed(m_particles);
  dynamic_object_functions::erase_destroyed(m_enemyObjects);
  dynamic_object_functions::erase_destroyed(m_playerObjects);
}

auto level_container::DoCollisions() -> void
{
  level_collision_handler collisionHandler { this };
  m_cellCollisionTests.particles(m_cells, m_particles, collisionHandler);
  m_cellCollisionTests(m_cells, m_playerObjects, collisionHandler);
  m_cellCollisionTests(m_cells, m_enemyObjects, collisionHandler);
  m_collisionRunner(m_playerObjects, m_enemyObjects, collisionHandler);
}

auto level_container::GetTargettedObject() -> std::optional<targetted_object>
{
#if 0
  constexpr auto angleSpan = 40.0f;
#endif

  auto targetableObjects = std::ranges::views::filter(m_enemyObjects, [](const auto& object)
  {
    return object->HoldsAlternative<enemy_type_1>();
  });

  dynamic_object<default_object>* nearestObject = std::accumulate(std::begin(targetableObjects), std::end(targetableObjects), 
  static_cast<dynamic_object<default_object>*>(nullptr), 
  [this](auto* nearest, auto& next) -> dynamic_object<default_object>*
  {
#if 0
    auto targetAngle = direct2d::GetAngleBetweenPoints(m_playerState.Position(), next->Position());
    auto angleDifference = direct2d::GetAngleDifference(m_playerState.Angle(), targetAngle);
    if( angleDifference < -angleSpan || angleDifference > angleSpan ) return nearest;
    else return nearest ? &GetNearestToTarget(*nearest, next) : &next;
#endif
    return nearest ? &GetNearestToTarget(*nearest, next) : &next;
  });

  return nearestObject ? std::optional<targetted_object>(nearestObject) : std::nullopt;
}

auto level_container::UpdateObject(player_ship& object, float interval) -> void
{
  object.Update(interval, m_cells);

  if( object.CanShoot() )
  {
    CreatePlayerBullet(object.Position(), direct2d::CalculateVelocity(1200, object.ShootAngle()), 1);
    play_events::set(play_events::event_type::shot, true);
  }

  if( object.EmitThrustParticle() )
  {
    auto thrustAngle = object.ThrustParticleDirection();
    auto thrustPosition = direct2d::CalculatePosition(object.Position(), thrustAngle, 40);
    auto thrustVelocity = direct2d::CombineVelocities(object.Velocity(), direct2d::CalculateVelocity(200.0f, thrustAngle));
    CreateParticle(particle::type::thrust, thrustPosition, thrustVelocity, 0.5f);
  }
}

auto level_container::UpdateObject(player_missile& object, float interval) -> void
{
  m_targettedObject ? object.Update(interval, m_targettedObject->Position()) : object.Update(interval);
}

auto level_container::UpdateObject(enemy_type_1& object, float interval) -> void
{
  object.Update(interval, m_playerState.Position(), m_cells);

  if( !m_playerState.Destroyed() && object.CanShootAt(m_playerState.Position()) )
  {
    auto direction = direct2d::GetAngleBetweenPoints(object.Position(), m_playerState.Position());
    auto velocity = direct2d::CalculateVelocity(500.0f, direction);
    CreateEnemyBullet(object.Position(), enemy_bullet_1::type::two, velocity);
    play_events::set(play_events::event_type::shot, true);
  }
}

auto level_container::UpdateObject(enemy_type_2& object, float interval) -> void
{
  object.Update(interval, m_cells);

  if( !m_playerState.Destroyed() && object.CanShootAt(m_playerState.Position()) )
  {
    auto direction = direct2d::GetAngleBetweenPoints(object.Position(), m_playerState.Position());
    auto velocity = direct2d::CalculateVelocity(600.0f, direction);
    CreateEnemyBullet(object.Position(), enemy_bullet_1::type::two, velocity);
    play_events::set(play_events::event_type::shot, true);
  }
}
