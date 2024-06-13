#include "pch.h"
#include "level_container.h"
#include "renderers.h"
#include "particle_functions.h"
#include "player_state.h"

level_container::level_container() : level_container(collision_type::boundary)
{
}

level_container::level_container(collision_type collisionType) : 
  m_playerState{{0, 0}, {1, 1}, 0, {0, 0}}, m_collisionTest { collisionType }, m_containmentTest { collisionType }, 
  m_defaultObjectBuffer { 128, 0 },
  m_defaultObjectAllocator { m_defaultObjectBuffer },
  m_noninteractiveObjects { m_defaultObjectAllocator },
  m_cellObjects { m_defaultObjectAllocator },
  m_playerObjects { m_defaultObjectAllocator },
  m_enemyObjects { m_defaultObjectAllocator },
  m_particleBuffer { 64, 0 },
  m_particleAllocator { m_particleBuffer },
  m_particles { m_particleAllocator },
  m_playerGeometries { 50 },
  m_enemyGeometries { 50 },
  m_wallGeometries { 100 }
{
}

auto level_container::Create(object_type objectType, POINT_2F position, SCALE_2F scale, float angle) -> default_object&
{
  switch( objectType )
  {
    case object_type::portal_entry:
      return CreateNoninteractiveObject(std::in_place_type<portal>, position, { 1, 1 }, 0);
    case object_type::portal_exit:
      return CreateEnemyObject(std::in_place_type<portal>, position, { 1, 1 }, 0, { 0, 0 });
    case object_type::player:
    {
      auto& defaultObject = CreatePlayerObject(std::in_place_type<player_ship>, position, { 1, 1 }, 0, { 0, 0 });
      auto* player = defaultObject.GetIf<player_ship>();
      m_playerState = *player;
      return defaultObject;
    }
    case object_type::enemy_stalker:
      return CreateEnemyObject(std::in_place_type<enemy_type_1>, position, { 1, 1 }, 0, { 0, 0 });
    case object_type::enemy_random:
      return CreateEnemyObject(std::in_place_type<enemy_type_2>, position, { 1, 1 }, 0, { 0, 0 });
    case object_type::enemy_turret:
      return CreateEnemyObject(std::in_place_type<enemy_type_3>, position, { 1, 1 }, 0, { 0, 0 });
    case object_type::power_up:
      return CreateEnemyObject(std::in_place_type<power_up>, position, { 1, 1 }, 0, { 0, 0 });
    case object_type::cell:
      return CreateCellObject(std::in_place_type<level_cell>, position, scale, angle, VELOCITY_2F { 0, 0 });
    default:
      return CreateEnemyObject(std::in_place_type<power_up>, position, { 1, 1 }, 0, { 0, 0 });
  }
}

auto level_container::CreatePlayerObject(auto variantType, POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) -> default_object&
{
  return m_playerObjects.emplace_back(variantType, position, scale, angle, velocity);
}

auto level_container::CreateEnemyObject(auto variantType, POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) -> default_object&
{
  return m_enemyObjects.emplace_back(variantType, position, scale, angle, velocity);
}

auto level_container::CreatePlayerBullet(POINT_2F position, SCALE_2F scale, float angle, float speed) -> default_object&
{
  auto velocity = direct2d::CalculateVelocity(speed, angle);
  return CreatePlayerObject(std::in_place_type<player_bullet>, position, scale, angle, velocity);
}

auto level_container::CreateEnemyBullet(POINT_2F position, SCALE_2F scale, float angle, float speed) -> default_object&
{
  auto velocity = direct2d::CalculateVelocity(speed, angle);
  return CreateEnemyObject(std::in_place_type<enemy_bullet_1>, position, scale, angle, velocity);
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

  auto wallCells = std::ranges::views::filter(m_cellObjects, [](auto& object)
  {
    auto* cellObject = object.GetIf<level_cell>();
    return cellObject && cellObject->Type() == level_cell_type::wall;
  });

  m_wallGeometries.Update(wallCells);
  m_playerGeometries.Update(m_playerObjects);
  m_enemyGeometries.Update(m_enemyObjects);

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
  
  diagnostics::add(L"default object - allocated buffers", static_cast<int>(m_defaultObjectBuffer.AllocationCount()));
  diagnostics::add(L"default object - heap allocations", static_cast<int>(m_defaultObjectBuffer.HeapAllocationCount()));
  
  diagnostics::add(L"particle - allocated buffers", static_cast<int>(m_particleBuffer.AllocationCount()));
  diagnostics::add(L"particle - heap allocations", static_cast<int>(m_particleBuffer.HeapAllocationCount()));
}

auto level_container::UpdateObjects(float interval) -> void
{
  auto particles = std::ranges::views::filter(m_particles, [](const auto& particle) { return !particle.Destroyed(); } );

  for( auto& particle : particles )
  {
    particle.Update(interval);
  }

  EnumerateAllObjects(false, [this, interval](auto& defaultObject)
  {
    std::visit([this, interval](auto& object) { UpdateObject(object, interval); }, defaultObject.Get());
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
  m_particleCollisionRunner(m_particles, m_wallGeometries, true);

  m_compare(m_playerGeometries, m_wallGeometries, [this](auto& object1, auto& object2)
  {
    if( m_collisionTest(object1, object2) )
    {
      OnCollision<player_bullet, level_cell>(object1.Object(), object2.Object());
    }
  });

  m_compare(m_enemyGeometries, m_wallGeometries, [this](auto& object1, auto& object2)
  {
    if( m_collisionTest(object1, object2) )
    {
      OnCollision<enemy_bullet_1, level_cell>(object1.Object(), object2.Object());
    }
  });

  m_compare(m_playerGeometries, m_enemyGeometries, [this](default_object_geometry& object1, default_object_geometry& object2)
  {
    if( m_collisionTest(object1, object2) )
    {
      OnCollision<player_bullet, enemy_type_1>(object1.Object(), object2.Object());
      OnCollision<player_bullet, enemy_type_2>(object1.Object(), object2.Object());
      OnCollision<player_bullet, enemy_type_3>(object1.Object(), object2.Object());
      OnCollision<player_ship, enemy_type_1>(object1.Object(), object2.Object());
      OnCollision<player_ship, enemy_type_2>(object1.Object(), object2.Object());
      OnCollision<player_ship, enemy_type_3>(object1.Object(), object2.Object());
      OnCollision<player_ship, enemy_bullet_1>(object1.Object(), object2.Object());
      OnCollision<player_ship, power_up>(object1.Object(), object2.Object());
    }
    
    if( m_containmentTest(object1, object2) )
    {
      OnContainment<player_ship, portal>(object1.Object(), object2.Object());
    }
  });
}

auto level_container::UpdateObject(player_ship& object, float interval) -> void
{
  object.Update(interval);

  if( object.CanShoot() )
  {
    CreatePlayerBullet(object.Position(), { 1, 1 }, object.ShootAngle(), 1500);
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

auto level_container::UpdateObject(enemy_type_1& object, float interval) -> void
{
  object.Update(interval, m_playerState.Position());

  if( !m_playerState.Destroyed() && object.CanShootAt(m_playerState.Position()) )
  {
    auto angle = direct2d::GetAngleBetweenPoints(object.Position(), m_playerState.Position());
    CreateEnemyBullet(object.Position(), { 1, 1 }, angle, 500);
    play_events::set(play_events::event_type::shot, true);
  }
}

auto level_container::UpdateObject(enemy_type_2& object, float interval) -> void
{
  object.Update(interval);

  if( !m_playerState.Destroyed() && object.CanShootAt(m_playerState.Position()) )
  {
    auto angle = direct2d::GetAngleBetweenPoints(object.Position(), m_playerState.Position());
    CreateEnemyBullet(object.Position(), { 1, 1 }, angle, 750);
    play_events::set(play_events::event_type::shot, true);
  }
}

auto level_container::UpdateObject(enemy_type_3 &object, float interval) -> void
{
  object.Update(interval, m_playerState.Position());

  if( !m_playerState.Destroyed() && object.CanShootAt(m_playerState.Position()) )
  {
    auto angle = direct2d::GetAngleBetweenPoints(object.Position(), m_playerState.Position());
    CreateEnemyBullet(object.Position(), { 1, 1 }, angle, 500);
    play_events::set(play_events::event_type::shot, true);
  }
}

auto level_container::OnCollision(player_bullet& bullet, enemy_type_1& enemy) -> void
{
  enemy.ApplyDamage(bullet.Damage());
  bullet.Destroy();
}

auto level_container::OnCollision(player_bullet& bullet, enemy_type_2& enemy) -> void
{
  bullet.Destroy();
  enemy.ApplyDamage(bullet.Damage());
}

auto level_container::OnCollision(player_bullet& bullet, enemy_type_3& enemy) -> void
{
  bullet.Destroy();
  enemy.ApplyDamage(bullet.Damage());
}

auto level_container::OnCollision(player_bullet &bullet, level_cell &wall) -> void
{
  bullet.Destroy();
}

auto level_container::OnCollision(enemy_bullet_1 &bullet, level_cell &wall) -> void
{
  bullet.Destroy();
}

auto level_container::OnCollision(player_ship& ship, enemy_type_1& enemy) -> void
{
  switch( player_state::get_status() )
  {
    case player_state::status::active:
      ship.Destroy();
      break;
    case player_state::status::celebrating:
      break;
  }

  enemy.Destroy();
}

auto level_container::OnCollision(player_ship& ship, enemy_type_2& enemy) -> void
{
  switch( player_state::get_status() )
  {
    case player_state::status::active:
      ship.Destroy();
      break;
    case player_state::status::celebrating:
      break;
  }

  enemy.Destroy();
}

auto level_container::OnCollision(player_ship& ship, enemy_type_3& enemy) -> void
{
  switch( player_state::get_status() )
  {
    case player_state::status::active:
      ship.Destroy();
      break;
    case player_state::status::celebrating:
      break;
  }

  enemy.Destroy();
}

auto level_container::OnCollision(player_ship& playerShip, enemy_bullet_1& enemyBullet) -> void
{
  switch( player_state::get_status() )
  {
    case player_state::status::active:
      playerShip.Destroy();
      break;
    case player_state::status::celebrating:
      break;
  }

  enemyBullet.Destroy();
}

auto level_container::OnCollision(player_ship& playerShip, power_up& powerUp) -> void
{
  powerUp.Destroy();
}

auto level_container::OnContainment(player_ship &player, portal &portalObj) -> void
{
  auto exitCell = portalObj.CellId();
  m_exit = exitCell ? true : false;
  m_exitCell = exitCell ? *exitCell : cell_id {};
}
