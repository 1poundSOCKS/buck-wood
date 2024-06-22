#include "pch.h"
#include "level_container.h"
#include "renderers.h"
#include "particle_functions.h"
#include "player_state.h"

level_container::level_container() : level_container(collision_type::boundary)
{
}

level_container::level_container(collision_type collisionType) : 
  m_cells { std::make_shared<level_cell_collection>(cell_size { 250, 250 }) },
  m_objectMovement { std::make_shared<level_object_movement>(m_cells) },
  m_playerState { std::make_shared<player_ship_state>(POINT_2F {0, 0}, SCALE_2F {1, 1}, 0.0f) }, 
  m_collisionRunner { collisionType }
{
}

auto level_container::AddObject(object_type objectType, POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) -> default_object&
{
  switch( objectType )
  {
    case object_type::portal_entry:
      return m_objects.Add(std::in_place_type<portal>, position, scale, angle, velocity);
    case object_type::portal_exit:
      return m_objects.Add(std::in_place_type<portal>, position, scale, angle, velocity);
    case object_type::player:
    {
      auto& defaultObject = m_objects.Add(std::in_place_type<player_ship>, position, scale, angle, velocity);
      defaultObject.Visit([this](auto& object) { OnAddObject(object); });
      return defaultObject;
    }
    case object_type::enemy_stalker:
      return m_objects.Add(std::in_place_type<enemy_type_1>, position, scale, angle, velocity);
    case object_type::enemy_random:
      return m_objects.Add(std::in_place_type<enemy_type_2>, position, scale, angle, velocity);
    case object_type::enemy_turret:
      return m_objects.Add(std::in_place_type<enemy_type_3>, position, scale, angle, velocity);
    case object_type::power_up:
      return m_objects.Add(std::in_place_type<power_up>, position, scale, angle, velocity);
    case object_type::cell:
      return m_objects.Add(std::in_place_type<level_cell>, position, scale, angle, velocity);
    default:
      return m_objects.Add(std::in_place_type<power_up>, position, scale, angle, velocity);
  }
}

auto level_container::AddWall(cell_id cellId, level_cell_type cellType) -> void
{
  auto cellSize = cell_size { 250, 250 };
  auto& object = m_objects.Add(std::in_place_type<level_cell>, ToFloat(cellSize.CellPosition(cellId)), { 250, 250 }, 0, { 0, 0 });
  auto wall = object.GetIf<level_cell>();
  wall->SetId(cellId);
  wall->SetType(cellType);

  m_cells->Set(cellId, cellType);
}

auto level_container::Update(float interval, D2D1_RECT_F viewRect) -> void
{
  auto updateStart = performance_counter::QueryValue();

  m_particles.Update(interval);

  m_objects.Visit([this,interval](auto& object) { UpdateObject(object, interval); });
  m_objects.Visit([this,interval](auto& object) { VisitObject(object); });

  auto collisionsStart = performance_counter::QueryValue();

  m_collisionGeometry.Update(m_objects);
  DoCollisions();
  
  auto collisionsEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"collisions", collisionsEnd - collisionsStart, game_settings::swapChainRefreshRate());

  auto destroyedObjects = std::ranges::views::filter(m_objects, [](const auto& object) { return object.Destroyed(); });

  for( const auto& object : destroyedObjects )
  {
    m_particles.Add(level_explosion { object.Position() });
    play_events::set(play_events::event_type::explosion, true);
  }

  m_particles.EraseDestroyed();
  m_objects.EraseDestroyed();

  auto enemies = std::ranges::views::transform(m_objects, [](const auto& object)
  {
    return object.HoldsAlternative<enemy_type_1>() || 
      object.HoldsAlternative<enemy_type_2>() || 
      object.HoldsAlternative<enemy_type_3>();
  });

  m_enemyCount = std::ranges::distance(enemies);

  auto updateEnd = performance_counter::QueryValue();
  
  diagnostics::addTime(L"level_container::update", updateEnd - updateStart, game_settings::swapChainRefreshRate());
}

auto level_container::DoCollisions() -> void
{
  m_particleCollisionRunner(m_particles, m_collisionGeometry(level_collision_geometry::type::wall), true);

  m_collisionRunner(m_collisionGeometry(level_collision_geometry::type::player), m_collisionGeometry(level_collision_geometry::type::wall), 
    [this](auto& object1, auto&object2, geometry_collision::result resultType) { OnCollision(object1, object2, resultType); });

  m_collisionRunner(m_collisionGeometry(level_collision_geometry::type::enemy), m_collisionGeometry(level_collision_geometry::type::wall), 
    [this](auto& object1, auto&object2, geometry_collision::result resultType) { OnCollision(object1, object2, resultType); });

  m_collisionRunner(m_collisionGeometry(level_collision_geometry::type::player), m_collisionGeometry(level_collision_geometry::type::enemy), 
    [this](auto& object1, auto&object2, geometry_collision::result resultType) { OnCollision(object1, object2, resultType); });
}

auto level_container::OnAddObject(player_ship &object) -> void
{
  m_playerState = object.State();
}

auto level_container::UpdateObject(player_ship &object, float interval) -> void
{
  object.Update(interval);

  auto leftThumbstickPosition = gamepad_reader::left_thumbstick();

  if( leftThumbstickPosition )
  {
    auto changeInVelocity = player_ship::CalculateVelocity(leftThumbstickPosition->x, leftThumbstickPosition->y);
    object.UpdateVelocity(changeInVelocity, interval);
    object.UpdateAngle();
  }

  constexpr SIZE_F objectSize { 60, 60 };

  auto initialPosition = object.Position();
  auto updatedPosition = object.UpdatePosition(interval);
  auto moveDistance = POINT_2F { updatedPosition.x - initialPosition.x, updatedPosition.y - initialPosition.y };

  auto adjustedPosition = m_objectMovement->UpdatePosition(initialPosition, moveDistance, objectSize);
  m_playerState->SetPosition(adjustedPosition);

  auto collisionX = updatedPosition.x != adjustedPosition.x;
  auto collisionY = updatedPosition.y != adjustedPosition.y;

  auto velocity = object.Velocity();
  auto velocityX = collisionX ? 0 : velocity.x;
  auto velocityY = collisionY ? 0 : velocity.y;

  constexpr float friction { 3.0f };
  auto intervalFriction = std::max(0.0f, 1.0f  - (friction * interval));
  velocityX *= intervalFriction;
  velocityY *= intervalFriction;
  m_playerState->SetVelocity({velocityX, velocityY});
}

auto level_container::UpdateObject(enemy_type_1 &object, float interval) -> void
{
  // object.SetCells(m_cells);
  object.Update(interval, *m_cells);
}

auto level_container::UpdateObject(enemy_type_2 &object, float interval) -> void
{
  // object.SetCells(m_cells);
  object.Update(interval, *m_cells);
}

auto level_container::UpdateObject(enemy_type_3 &object, float interval) -> void
{
  // object.SetCells(m_cells);
  object.Update(interval);
}

auto level_container::VisitObject(player_ship &object) -> void
{
  if( object.CanShoot() )
  {
    std::optional<D2D1_POINT_2F> rightThumbstickPosition = gamepad_reader::right_thumbstick();

    if( rightThumbstickPosition )
    {
      auto shootAngle = static_cast<int>(direct2d::GetAngleBetweenPoints({0,0}, *rightThumbstickPosition));
      shootAngle += 45;
      shootAngle /= 90;
      shootAngle *= 90;
      auto modifiedShootAngle = static_cast<float>(shootAngle);
      m_objects.Add(std::in_place_type<player_bullet>, object.Position(), { 1, 1 }, modifiedShootAngle, direct2d::CalculateVelocity(2000, modifiedShootAngle));
      play_events::set(play_events::event_type::shot, true);
    }
  }
}

auto level_container::VisitObject(enemy_type_1& object) -> void
{
  object.SetTarget(m_playerState->Position());

  if( !m_playerState->Destroyed() && object.CanShootAt(m_playerState->Position()) )
  {
    auto angle = direct2d::GetAngleBetweenPoints(object.Position(), m_playerState->Position());
    m_objects.Add(std::in_place_type<enemy_bullet_1>, object.Position(), { 1, 1 }, angle, direct2d::CalculateVelocity(500, angle));
    play_events::set(play_events::event_type::shot, true);
  }
}

auto level_container::VisitObject(enemy_type_2& object) -> void
{
  if( !m_playerState->Destroyed() && object.CanShootAt(m_playerState->Position()) )
  {
    auto angle = direct2d::GetAngleBetweenPoints(object.Position(), m_playerState->Position());
    m_objects.Add(std::in_place_type<enemy_bullet_1>, object.Position(), { 1, 1 }, angle, direct2d::CalculateVelocity(750, angle));
    play_events::set(play_events::event_type::shot, true);
  }
}

auto level_container::VisitObject(enemy_type_3 &object) -> void
{
  object.SetTarget(m_playerState->Position());

  if( !m_playerState->Destroyed() && object.CanShootAt(m_playerState->Position()) )
  {
    auto angle = direct2d::GetAngleBetweenPoints(object.Position(), m_playerState->Position());
    m_objects.Add(std::in_place_type<enemy_bullet_1>, object.Position(), { 1, 1 }, angle, direct2d::CalculateVelocity(500, angle));
    play_events::set(play_events::event_type::shot, true);
  }
}

auto level_container::OnCollision(player_bullet& bullet, enemy_type_1& enemy, geometry_collision::result result) -> void
{
  enemy.ApplyDamage(bullet.Damage());
  bullet.Destroy();
}

auto level_container::OnCollision(player_bullet& bullet, enemy_type_2& enemy, geometry_collision::result result) -> void
{
  bullet.Destroy();
  enemy.ApplyDamage(bullet.Damage());
}

auto level_container::OnCollision(player_bullet& bullet, enemy_type_3& enemy, geometry_collision::result result) -> void
{
  bullet.Destroy();
  enemy.ApplyDamage(bullet.Damage());
}

auto level_container::OnCollision(player_bullet &bullet, level_cell &wall, geometry_collision::result result) -> void
{
  bullet.Destroy();
}

auto level_container::OnCollision(enemy_bullet_1 &bullet, level_cell &wall, geometry_collision::result result) -> void
{
  bullet.Destroy();
}

auto level_container::OnCollision(player_ship& ship, enemy_type_1& enemy, geometry_collision::result result) -> void
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

auto level_container::OnCollision(player_ship& ship, enemy_type_2& enemy, geometry_collision::result result) -> void
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

auto level_container::OnCollision(player_ship& ship, enemy_type_3& enemy, geometry_collision::result result) -> void
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

auto level_container::OnCollision(player_ship& playerShip, enemy_bullet_1& enemyBullet, geometry_collision::result result) -> void
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

auto level_container::OnCollision(player_ship& playerShip, power_up& powerUp, geometry_collision::result result) -> void
{
  powerUp.Destroy();
}

auto level_container::OnCollision(player_ship &player, portal &portalObj, geometry_collision::result result) -> void
{
  if( result == geometry_collision::result::containment )
  {
    auto exitCell = portalObj.CellId();
    m_exit = exitCell ? true : false;
    m_exitCell = exitCell ? *exitCell : cell_id {};
  }
}
