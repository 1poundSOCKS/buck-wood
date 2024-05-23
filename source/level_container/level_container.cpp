#include "pch.h"
#include "level_container.h"
#include "renderers.h"
#include "particle_functions.h"
#include "level_collision_handler.h"
#include "player_state.h"

auto level_container::AddFloorCell(int x, int y, level_cell_type cellType) -> void
{
  m_cells.Add(x, y, cellType);
}

auto level_container::AddWalls() -> void
{
  m_cells.AddWalls();

  m_cells.EnumerateWalls([](const auto& wall)
  {
  });

  m_enemyCollisionObjects.clear();

  EnumerateWallObjects([this](auto& object)
  {
    m_wallCollisionObjects.emplace_back(object);
  });
}

auto level_container::Update(float interval, D2D1_RECT_F viewRect) -> void
{
  auto collisionsStart = performance_counter::QueryValue();
  DoCollisions();
  auto collisionsEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"collisions", collisionsEnd - collisionsStart, game_settings::swapChainRefreshRate());

  auto updateStart = performance_counter::QueryValue();
  UpdateObjects(interval);

  for( const auto& object : m_playerObjects )
  {
    auto ship = object.GetIf<player_ship>();
    m_playerState = ship ? *ship : m_playerState;
  }

  EnumerateAllObjects(true, [this](const auto& object)
  {
    if( object.Destroyed() )
    {
      CreateExplosion(object.Position());
    }
  });

  RemoveDestroyedObjects();

  m_playerCollisionObjects.clear();

  EnumeratPlayerObjects(false, [this](auto& object)
  {
    m_playerCollisionObjects.emplace_back(object);
  });

  m_enemyCollisionObjects.clear();

  EnumerateEnemyObjects(false, [this](auto& object)
  {
    m_enemyCollisionObjects.emplace_back(object);
  });

#if 0
  m_targettedObject = m_playerState.TargettingActive() ? GetTargettedObject() : std::nullopt;
#endif

  auto enemies = std::ranges::views::transform(m_enemyObjects, [](const auto& object)
  {
    return std::holds_alternative<enemy_type_1>(object.Get()) || std::holds_alternative<enemy_type_2>(object.Get()) ? 1 : 0;
  });

  m_enemyCount = m_enemyObjects.size();

  auto updateEnd = performance_counter::QueryValue();
  diagnostics::addTime(L"level_container::update", updateEnd - updateStart, game_settings::swapChainRefreshRate());
}

auto level_container::UpdateObjects(float interval) -> void
{
  auto particles = std::ranges::views::filter(m_particles, [](const auto& particle) { return !particle.Destroyed(); } );

  dynamic_object_functions::update(particles, interval);

  EnumerateAllObjects(false, [this, interval](auto& defaultObject)
  {
    std::visit([this, interval](auto& object)
    {
      UpdateObject(object, interval);
    }, defaultObject.Get());
  });
}

auto level_container::RemoveDestroyedObjects() -> void
{
  std::erase_if(m_particles, [](const auto& particle) -> bool { return particle.Destroyed(); });
  std::erase_if(m_enemyObjects, [](const auto& object) -> bool { return object.Destroyed(); });
  std::erase_if(m_playerObjects, [](const auto& object) -> bool { return object.Destroyed(); });
}

auto level_container::DoCollisions() -> void
{
  level_collision_handler collisionHandler;
  // m_cellCollisionTests.particles(m_cells, m_particles, collisionHandler);
  // m_cellCollisionTests(m_cells, m_playerObjects, collisionHandler);
  // m_cellCollisionTests(m_cells, m_enemyObjects, collisionHandler);
  m_collisionRunner(m_playerCollisionObjects, m_enemyCollisionObjects, collisionHandler);

  m_collisionRunner(m_playerCollisionObjects, m_wallCollisionObjects, collisionHandler);
  m_collisionRunner(m_enemyCollisionObjects, m_wallCollisionObjects, collisionHandler);

  // auto exitCell = collisionHandler.ExitCell();
  // m_exit = exitCell ? true : false;
  // m_exitCell = exitCell ? *exitCell : POINT_2I { 0, 0 };
}

#if 0
auto level_container::GetTargettedObject() -> std::optional<targetted_object>
{
#if 0
  constexpr auto angleSpan = 40.0f;
#endif

  // auto targetableObjects = std::ranges::views::filter(m_enemyObjects, [](const auto& object)
  // {
  //   return object->HoldsAlternative<enemy_type_1>();
  // });

  auto targetableObjects = std::ranges::views::filter(m_enemyObjects, [](const auto& object)
  {
    return object.HoldsAlternative<enemy_type_1>();
  });

  // dynamic_object<default_object>* nearestObject = std::accumulate(std::begin(targetableObjects), std::end(targetableObjects), 
  default_object* nearestObject = std::accumulate(std::begin(targetableObjects), std::end(targetableObjects), 
  // static_cast<dynamic_object<default_object>*>(nullptr), 
  static_cast<default_object*>(nullptr), 
  // [this](auto* nearest, auto& next) -> dynamic_object<default_object>*
  [this](auto* nearest, auto& next) -> default_object*
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
#endif

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

auto level_container::UpdateObject(enemy_type_3 &object, float interval) -> void
{
  object.Update(interval);

  if( !m_playerState.Destroyed() && object.CanShootAt(m_playerState.Position()) )
  {
    auto direction = direct2d::GetAngleBetweenPoints(object.Position(), m_playerState.Position());
    auto velocity = direct2d::CalculateVelocity(600.0f, direction);
    CreateEnemyBullet(object.Position(), enemy_bullet_1::type::two, velocity);
    play_events::set(play_events::event_type::shot, true);
  }
}
