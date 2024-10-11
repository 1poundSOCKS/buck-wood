#include "pch.h"
#include "level_container.h"
#include "renderers.h"
#include "particle_functions.h"
#include "player_state.h"
#include "visitor.h"

level_container::level_container() : level_container(collision_type::direct2d)
{
}

level_container::level_container(collision_type collisionType) : 
  m_cells { std::make_shared<level_cell_collection>(cell_size { m_cellSize, m_cellSize }, RECT_I { 0, 0, 10, 10 }) },
  m_objectMovement { std::make_shared<level_object_movement>(m_cells) },
  m_playerState { std::make_shared<player_ship_state>(POINT_2F {0, 0}, SCALE_2F {1, 1}, 0.0f, VELOCITY_2F { 0, 0 }) }, 
  m_controller { m_playerState },
  m_collisionRunner { collisionType },
  m_boundary { 0.0f, 0.0f, 0.0f, 0.0f }
{
}

auto level_container::AddObject(object_type objectType, cell_id cellId) -> default_object &
{
  auto& object = AddObject(objectType, ToFloat(m_cells->CellSize().CellPosition(cellId)), { 1.0f, 1.0f }, 0.0f, { 0.0f, 0.0f });

  object.Visit( visitor {
    [cellId](portal& object) { object.SetCellId(cellId); },
    [cellId](player_ship& object) { object.State()->SetPosition(ToFloat(cellId.Position(m_cellSize, m_cellSize))); },
    [cellId](enemy_ship& object) { object.SetCellId(cellId); },
    [](auto& object) {}
  });

  return object;
}

auto level_container::AddCell(cell_id cellId, level_cell_type cellType) -> void
{
  m_cells->Set(cellId, cellType);

  auto cellSize = cell_size { m_cellSize, m_cellSize };
  auto cellBounds = m_cells->Bounds();

  m_boundary.left = static_cast<float>(cellBounds.Left(cellSize)) - m_cellSize / 2;
  m_boundary.top = static_cast<float>(cellBounds.Top(cellSize)) - m_cellSize / 2;
  m_boundary.right = static_cast<float>(cellBounds.Right(cellSize)) + m_cellSize / 2;
  m_boundary.bottom = static_cast<float>(cellBounds.Bottom(cellSize)) + m_cellSize / 2;
}

auto level_container::AddBoundaryWalls() -> void
{
  auto position = POINT_2F { ( m_boundary.left + m_boundary.right ) / 2.0f , ( m_boundary.bottom + m_boundary.top ) / 2.0f };
  auto scale = SCALE_2F { m_boundary.right - m_boundary.left, m_boundary.bottom - m_boundary.top };
  m_objects.Add(std::in_place_type<boundary_walls>, position, scale, 0, 0);

  // auto leftVisitor = visitor {
  //   [](boundary_exit& object) { object.SetType(boundary_exit::type::left); },
  //   [](auto&& object) { }
  // };

  // auto topVisitor = visitor {
  //   [](boundary_exit& object) { object.SetType(boundary_exit::type::top); },
  //   [](auto&& object) { }
  // };

  // auto rightVisitor = visitor {
  //   [](boundary_exit& object) { object.SetType(boundary_exit::type::right); },
  //   [](auto&& object) { }
  // };

  // auto bottomVisitor = visitor {
  //   [](boundary_exit& object) { object.SetType(boundary_exit::type::bottom); },
  //   [](auto&& object) { }
  // };

  // m_objects.Add(std::in_place_type<boundary_exit>, position, scale, 0).Visit(leftVisitor);
  // m_objects.Add(std::in_place_type<boundary_exit>, position, scale, 0).Visit(topVisitor);
  // m_objects.Add(std::in_place_type<boundary_exit>, position, scale, 0).Visit(rightVisitor);
  // m_objects.Add(std::in_place_type<boundary_exit>, position, scale, 0).Visit(bottomVisitor);
}

auto level_container::UnoccupiedFloorCellCount() const noexcept -> size_t
{
  return m_cells->UnoccupiedFloorCellCount();
}

auto level_container::UnoccupiedFloorCell(size_t index) const noexcept -> cell_id
{
  return m_cells->UnnoccupiedFloorCell(index);
}

auto level_container::AddObject(object_type objectType, POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) -> default_object&
{
  switch( objectType )
  {
    case object_type::portal_entry:
      return m_objects.Add(std::in_place_type<portal>, position, scale, angle);
    case object_type::portal_exit:
      return m_objects.Add(std::in_place_type<portal>, position, scale, angle);
    case object_type::player:
    {
      auto& defaultObject = m_objects.Add(std::in_place_type<player_ship>, position, scale, angle, velocity);
      defaultObject.Visit( visitor {
        [this](player_ship& object) { m_playerState = object.State(); m_controller.SetState(object.State()); },
        [](auto& object) {}
      });
      return defaultObject;
    }
    case object_type::enemy_stalker:
      return m_objects.Add(std::in_place_type<enemy_ship>, position, scale, angle, enemy_ship::type::stalker);
    case object_type::enemy_random:
      return m_objects.Add(std::in_place_type<enemy_ship>, position, scale, angle, enemy_ship::type::random);
    case object_type::enemy_turret:
      return m_objects.Add(std::in_place_type<enemy_ship>, position, scale, angle, enemy_ship::type::turret);
    case object_type::power_up:
      return m_objects.Add(std::in_place_type<power_up>, position, scale, angle, velocity);
    case object_type::cell:
      return m_objects.Add(std::in_place_type<level_cell>, position, scale, angle);
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

  auto onDestroyed = visitor {
    [this](const player_ship& object)
    {
      m_particles.Add(level_explosion { object.Position() });
      play_events::set(play_events::event_type::explosion, true);
    },
    [this](const player_bullet& object)
    {
      m_particles.Create(particle::type::impact, object.Position(), { 0.0f, 0.0f }, 1.0f);
    },
    [this](const enemy_ship& object)
    {
      object.PreErase(*m_cells);
      m_particles.Add(level_explosion { object.Position() });
      play_events::set(play_events::event_type::explosion, true);
    },
    [this](const enemy_bullet& object)
    {
      m_particles.Add(level_explosion { object.Position() });
      play_events::set(play_events::event_type::explosion, true);
    },
    [this](const power_up& object)
    {
      play_events::increment(play_events::counter_type::power_ups_collected);
    },
    [this](const auto& object)
    {
    }
  };

  for( const auto& object : destroyedObjects )
  {
    object.Visit(onDestroyed);
  }

  m_particles.EraseDestroyed();
  m_objects.EraseDestroyed();

  auto enemies = std::ranges::views::transform(m_objects, [](const auto& object)
  {
    return object.HoldsAlternative<enemy_ship>();
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

  m_collisionRunner(m_collisionGeometry(level_collision_geometry::type::player), m_collisionGeometry(level_collision_geometry::type::boundary), 
    [this](auto& object1, auto&object2, geometry_collision::result resultType) { OnCollision(object1, object2, resultType); });

  m_collisionRunner(m_collisionGeometry(level_collision_geometry::type::enemy), m_collisionGeometry(level_collision_geometry::type::boundary), 
    [this](auto& object1, auto&object2, geometry_collision::result resultType) { OnCollision(object1, object2, resultType); });
}

auto level_container::UpdateObject(player_ship &object, float interval) -> void
{
  constexpr VELOCITY_2F forceOfGravity = { 0.0f, 400.0f };
  constexpr float airResistance = { 0.5f };
  
  m_controller.Update(interval);
  m_playerState->Update(forceOfGravity, airResistance, interval);
}

auto level_container::UpdateObject(enemy_ship &object, float interval) -> void
{
  object.Update(interval, m_playerState->Position(), *m_cells);
}

auto level_container::VisitObject(player_ship &object) -> void
{
  if( m_playerState->CanShoot() && m_playerState->ShootAngle() )
  {
    auto shootAngle = *m_playerState->ShootAngle();
    m_objects.Add(std::in_place_type<player_bullet>, object.Position(), { 1, 1 }, shootAngle, direct2d::CalculateVelocity(2500, shootAngle));
    play_events::set(play_events::event_type::shot, true);
  }
}

auto level_container::VisitObject(enemy_ship& object) -> void
{
  if( !m_playerState->Destroyed() && object.CanShootAt(m_playerState->Position()) )
  {
    auto angle = direct2d::GetAngleBetweenPoints(object.Position(), m_playerState->Position());
    m_objects.Add(std::in_place_type<enemy_bullet>, object.Position(), { 1, 1 }, angle, direct2d::CalculateVelocity(1000, angle));
    play_events::set(play_events::event_type::shot, true);
  }
}

auto level_container::OnCollision(player_bullet& bullet, enemy_ship& enemy, geometry_collision::result result) -> void
{
  if( result != geometry_collision::result::none )
  {
    enemy.ApplyDamage(bullet.Damage());
    bullet.Destroy();
  }
}

auto level_container::OnCollision(player_bullet &bullet, level_cell &wall, geometry_collision::result result) -> void
{
  if( result != geometry_collision::result::none )
  {
    bullet.Destroy();
  }
}

auto level_container::OnCollision(enemy_bullet &bullet, level_cell &wall, geometry_collision::result result) -> void
{
  if( result != geometry_collision::result::none )
  {
    bullet.Destroy();
  }
}

auto level_container::OnCollision(player_ship& ship, enemy_ship& enemy, geometry_collision::result result) -> void
{
  if( result != geometry_collision::result::none )
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
}

auto level_container::OnCollision(player_ship& playerShip, enemy_bullet& enemyBullet, geometry_collision::result result) -> void
{
  if( result != geometry_collision::result::none )
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
}

auto level_container::OnCollision(player_ship& playerShip, power_up& powerUp, geometry_collision::result result) -> void
{
  if( result != geometry_collision::result::none )
  {
    powerUp.Destroy();
  }
}

auto level_container::OnCollision(player_ship &player, portal &portalObj, geometry_collision::result result) -> void
{
  if( result != geometry_collision::result::none )
  {
    auto exitCell = portalObj.CellId();
    m_exit = exitCell ? true : false;
    m_exitCell = exitCell ? *exitCell : cell_id {};
  }
}
