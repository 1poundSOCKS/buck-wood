#include "pch.h"
#include "level_container.h"
#include "renderers.h"
#include "particle_functions.h"
#include "player_state.h"
#include "visitor.h"

level_container::level_container() : level_container(range_comparison_runner::execution::sequential, collision_type::direct2d)
{
}

level_container::level_container(range_comparison_runner::execution ex, collision_type collisionType) : 
  m_cells { std::make_shared<level_cell_collection>(cell_size { m_cellSize, m_cellSize }, RECT_I { 0, 0, 10, 10 }) },
  m_objectMovement { std::make_shared<level_object_movement>(m_cells) },
  m_playerState { std::make_shared<player_ship_state>(POINT_2F {0, 0}, SCALE_2F {1, 1}, 0.0f, VELOCITY_2F { 0, 0 }) }, 
  m_collisionRunner { ex, collisionType },
  m_boundary { 0.0f, 0.0f, 0.0f, 0.0f }
{
}

auto level_container::InitializePlayer() -> void
{
  for( auto&& object : m_objects )
  {
    object.Visit( visitor {
      [this](player_ship& object) { m_playerState = object.State(); },
      [](auto& object) {}
  });
  }
}

auto level_container::UnoccupiedFloorCellCount() const noexcept -> size_t
{
  return m_cells->UnoccupiedFloorCellCount();
}

auto level_container::UnoccupiedFloorCell(size_t index) const noexcept -> cell_id
{
  return m_cells->UnnoccupiedFloorCell(index);
}

auto level_container::Update(float interval, D2D1_RECT_F viewRect) -> std::optional<player_ship_state>
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

  if( ObjectCount(object_type::power_up) == 0 )
  {
    m_playerState->Celebrate();
  }

  auto updateEnd = performance_counter::QueryValue();
  
  diagnostics::addTime(L"level_container::update", updateEnd - updateStart, game_settings::swapChainRefreshRate());

  std::optional<player_ship_state> playerState;
  return std::accumulate(std::begin(m_objects), std::end(m_objects), playerState, [](std::optional<player_ship_state> playerState, const default_object& object)
  {
    auto&& playerShip = object.GetIf<player_ship>();
    return object.HoldsAlternative<player_ship>() ? std::optional<player_ship_state>(playerShip->StateValue()) : playerState;
  });
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
  
  player_controls::Update(m_playerState, interval);
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
    m_objects.Add(std::in_place_type<enemy_bullet>, object.Position(), { 1, 1 }, angle, direct2d::CalculateVelocity(1200, angle));
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
    if( !m_playerState->Celebrating() )
    {
      ship.Destroy();
    }

    enemy.Destroy();
  }
}

auto level_container::OnCollision(player_ship& playerShip, enemy_bullet& enemyBullet, geometry_collision::result result) -> void
{
  if( result != geometry_collision::result::none )
  {
    if( !m_playerState->Celebrating() )
    {
      playerShip.Destroy();
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
