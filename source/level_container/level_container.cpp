#include "pch.h"
#include "level_container.h"
#include "renderers.h"
#include "particle_functions.h"
#include "player_state.h"
#include "make_overload.h"

level_container::level_container() : level_container(collision_type::boundary)
{
}

level_container::level_container(collision_type collisionType) : 
  m_cells { std::make_shared<level_cell_collection>(cell_size { m_cellSize, m_cellSize }, RECT_I { 0, 0, 10, 10 }) },
  m_objectMovement { std::make_shared<level_object_movement>(m_cells) },
  m_playerState { std::make_shared<player_ship_state>(POINT_2F {0, 0}, SCALE_2F {1, 1}, 0.0f, VELOCITY_2F { 0, 0 }) }, 
  m_collisionRunner { collisionType }
{
}

auto level_container::AddObject(object_type objectType, cell_id cellId) -> default_object &
{
  auto& object = AddObject(objectType, ToFloat(m_cells->CellSize().CellPosition(cellId)), {1,1}, 0, {0,0});

  object.Visit(make_overload {
    [cellId](portal& object) { object.SetCellId(cellId); },
    [cellId](player_ship& object) { object.State()->SetCellId(cellId, cell_size { m_cellSize, m_cellSize }); },
    [](auto& object) {}
  });

  return object;
}

auto level_container::AddCell(cell_id cellId, level_cell_type cellType) -> void
{
  m_cells->Set(cellId, cellType);

  auto cellSize = cell_size { m_cellSize, m_cellSize };
  auto cellPosition = ToFloat(cellSize.CellPosition(cellId));
  auto cellScale = SCALE_2F { m_cellWidth, m_cellHeight };

  auto& object = m_objects.Add(std::in_place_type<level_cell>, cellPosition, cellScale, 0, { 0, 0 });
  auto wall = object.GetIf<level_cell>();
  wall->SetId(cellId);
  wall->SetType(cellType);
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
      defaultObject.Visit(make_overload {
        [this](player_ship& object) { m_playerState = object.State(); },
        [](auto& object) {}
      });
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

auto level_container::UpdateObject(player_ship &object, float interval) -> void
{
  m_playerState->StayPut();

  auto leftThumbstickPosition = gamepad_reader::left_thumbstick();

  if( leftThumbstickPosition )
  {
    enum class horizontal_move { none, left, right };
    enum class vertical_move { none, up, down };

    auto horizontalMove = horizontal_move::none;
    horizontalMove = leftThumbstickPosition->x > 0.3 ? horizontal_move::right : horizontalMove;
    horizontalMove = leftThumbstickPosition->x < -0.3 ? horizontal_move::left : horizontalMove;

    auto verticalMove = vertical_move::none;
    verticalMove = leftThumbstickPosition->y < -0.3 ? vertical_move::up : verticalMove;
    verticalMove = leftThumbstickPosition->y > 0.3 ? vertical_move::down : verticalMove;

    auto playerCellId = m_playerState->CellId();

    switch( horizontalMove )
    {
      case horizontal_move::left:
      {
        auto cellId = playerCellId.Get(cell_id::relative_position::left);
        m_cells->IsTypeOf(cellId, level_cell_type::wall) ? m_playerState->StayPut() : m_playerState->MoveLeft();
        break;
      }

      case horizontal_move::right:
      {
        auto cellId = playerCellId.Get(cell_id::relative_position::right);
        m_cells->IsTypeOf(cellId, level_cell_type::wall) ? m_playerState->StayPut() : m_playerState->MoveRight();
        break;
      }
    }

    switch( verticalMove )
    {
      case vertical_move::up:
      {
        auto cellId = playerCellId.Get(cell_id::relative_position::above);
        m_cells->IsTypeOf(cellId, level_cell_type::wall) ? m_playerState->StayPut() : m_playerState->MoveUp();
        break;
      }

      case vertical_move::down:
      {
        auto cellId = playerCellId.Get(cell_id::relative_position::below);
        m_cells->IsTypeOf(cellId, level_cell_type::wall) ? m_playerState->StayPut() : m_playerState->MoveDown();
        break;
      }
    }
  }

  m_playerState->Update(interval, cell_size { m_cellSize, m_cellSize });
}

auto level_container::UpdateObject(enemy_type_1 &object, float interval) -> void
{
  object.Update(interval, m_playerState->Position(), *m_cells);
}

auto level_container::UpdateObject(enemy_type_2 &object, float interval) -> void
{
  object.Update(interval, m_playerState->Position(), *m_cells);
}

auto level_container::UpdateObject(enemy_type_3 &object, float interval) -> void
{
  object.Update(interval, m_playerState->Position());
}

auto level_container::VisitObject(player_ship &object) -> void
{
  if( m_playerState->CanShoot() )
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
