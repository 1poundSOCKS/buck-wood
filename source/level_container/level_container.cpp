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
  m_playerState { POINT_2F { 0.0f, 0.0f }, SCALE_2F { 1.0f, 1.0f }, 0.0f, VELOCITY_2F { 0.0f, 0.0f } },  m_collisionRunner { ex, collisionType }
{
}

auto level_container::AddPlayer(POINT_2F position) noexcept -> void
{
  m_playerState.SetPosition(position);
  m_objects.AddPlayer(m_playerState);
}

auto level_container::Update(float interval, D2D1_RECT_F viewRect, bool levelComplete) -> void
{
  auto updateStart = performance_counter::QueryValue();

  m_particles.Update(interval);

  m_objects.Visit([this,interval,levelComplete](auto& object) { UpdateObject(object, interval, levelComplete); });
  m_objects.Visit([this,interval,levelComplete](auto& object) { VisitObject(object, levelComplete); });

  auto collisionsStart = performance_counter::QueryValue();

  m_collisionGeometry.Update(m_objects);
  DoCollisions(levelComplete);
  
  auto collisionsEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"collisions", collisionsEnd - collisionsStart, game_settings::swapChainRefreshRate());

  auto destroyedObjects = std::ranges::views::filter(m_objects, [](const auto& object) { return object.Destroyed(); });

  for( const auto& object : destroyedObjects )
  {
    object.Visit([this](auto&& object) { OnDestroyed(object); });
  }

  m_particles.EraseDestroyed();
  m_objects.EraseDestroyed();

  auto updateEnd = performance_counter::QueryValue();
  
  diagnostics::addTime(L"level_container::update", updateEnd - updateStart, game_settings::swapChainRefreshRate());
}

auto level_container::DoCollisions(bool levelComplete) -> void
{
  m_collisionRunner(m_collisionGeometry(level_collision_geometry::type::player), m_collisionGeometry(level_collision_geometry::type::wall), 
    [this, levelComplete](auto& object1, auto&object2, geometry_collision::result resultType) { OnCollision(object1, object2, resultType, levelComplete); });

  m_collisionRunner(m_collisionGeometry(level_collision_geometry::type::enemy), m_collisionGeometry(level_collision_geometry::type::wall), 
    [this, levelComplete](auto& object1, auto&object2, geometry_collision::result resultType) { OnCollision(object1, object2, resultType, levelComplete); });

  m_collisionRunner(m_collisionGeometry(level_collision_geometry::type::player), m_collisionGeometry(level_collision_geometry::type::enemy), 
    [this, levelComplete](auto& object1, auto&object2, geometry_collision::result resultType) { OnCollision(object1, object2, resultType, levelComplete); });

  m_collisionRunner(m_collisionGeometry(level_collision_geometry::type::player), m_collisionGeometry(level_collision_geometry::type::boundary), 
    [this, levelComplete](auto& object1, auto&object2, geometry_collision::result resultType) { OnCollision(object1, object2, resultType, levelComplete); });

  m_collisionRunner(m_collisionGeometry(level_collision_geometry::type::enemy), m_collisionGeometry(level_collision_geometry::type::boundary), 
    [this, levelComplete](auto& object1, auto&object2, geometry_collision::result resultType) { OnCollision(object1, object2, resultType, levelComplete); });
}

auto level_container::UpdateObject(player_ship &object, float interval, bool levelComplete) -> void
{
  constexpr VELOCITY_2F forceOfGravity = { 0.0f, 0.0f };
  constexpr float airResistance = { 0.6f };
  
  player_controls::Update(object.State(), interval);
  object.Update(forceOfGravity, airResistance, interval, levelComplete);
}

auto level_container::UpdateObject(enemy_ship &object, float interval, bool levelComplete) -> void
{
  object.Update(interval, m_playerState.Position());
}

auto level_container::VisitObject(player_ship &object, bool levelComplete) -> void
{
  if( object.State().CanShoot() && object.State().ShootAngle() && !levelComplete )
  {
    auto shootAngle = *(object.State().ShootAngle());
    m_objects.Add(std::in_place_type<player_bullet>, object.Position(), SCALE_2F { 1.0f, 1.0f }, shootAngle, direct2d::CalculateVelocity(2500, shootAngle));
    play_events::add(play_events::event_type::basic, play_events::basic_event_type::player_shot);
  }
}

auto level_container::VisitObject(enemy_ship& object, bool levelComplete) -> void
{
  if( !m_playerState.Destroyed() && object.CanShootAt(m_playerState.Position()) )
  {
    auto angle = direct2d::GetAngleBetweenPoints(object.Position(), m_playerState.Position());
    m_objects.Add(std::in_place_type<enemy_bullet>, object.Position(), SCALE_2F { 1.0f, 1.0f }, angle, direct2d::CalculateVelocity(1200, angle));
    play_events::add(play_events::event_type::basic, play_events::basic_event_type::enemy_shot);
  }
}

auto level_container::OnCollision(player_bullet& bullet, enemy_ship& enemy, geometry_collision::result result, bool levelComplete) -> void
{
  if( result != geometry_collision::result::none && !levelComplete )
  {
    enemy.ApplyDamage(bullet.Damage());
    bullet.Destroy();
  }
}

auto level_container::OnCollision(player_bullet &bullet, boundary_walls &boundaryWalls, geometry_collision::result result, bool levelComplete) -> void
{
  if( result != geometry_collision::result::containment )
  {
    m_particles.Create(particle::type::impact, bullet.Position(), { 0.0f, 0.0f }, 1.0f);
    bullet.Destroy();
  }
}

auto level_container::OnCollision(player_bullet &bullet, inner_walls &innerWalls, geometry_collision::result result, bool levelComplete) -> void
{
  if( result != geometry_collision::result::none )
  {
    m_particles.Create(particle::type::impact, bullet.Position(), { 0.0f, 0.0f }, 1.0f);
    bullet.Destroy();
  }
}

auto level_container::OnCollision(enemy_ship &enemyShip, boundary_walls &boundaryWalls, geometry_collision::result result, bool levelComplete) -> void
{
}

auto level_container::OnCollision(player_ship& player, enemy_ship& enemy, geometry_collision::result result, bool levelComplete) -> void
{
  if( result != geometry_collision::result::none && !levelComplete )
  {
    player.Destroy();
    enemy.Destroy();
  }
}

auto level_container::OnCollision(player_ship& player, enemy_bullet& enemyBullet, geometry_collision::result result, bool levelComplete) -> void
{
  if( result != geometry_collision::result::none  && !levelComplete )
  {
    player.Destroy();
    enemyBullet.Destroy();
  }
}

auto level_container::OnCollision(player_ship& playerShip, power_up& powerUp, geometry_collision::result result, bool levelComplete) -> void
{
  if( result != geometry_collision::result::none && !levelComplete )
  {
    powerUp.Destroy();
  }
}

auto level_container::OnDestroyed(const player_ship& object) -> void
{
  m_particles.Add(level_explosion { object.Position() });
  play_events::add(play_events::event_type::basic, play_events::basic_event_type::player_destroyed);
}

auto level_container::OnDestroyed(const enemy_ship& object) -> void
{
  m_particles.Add(level_explosion { object.Position() });
  play_events::add(play_events::event_type::basic, play_events::basic_event_type::enemy_destroyed);
}

auto level_container::OnDestroyed(const enemy_bullet& object) -> void
{
  m_particles.Add(level_explosion { object.Position() });
  play_events::add(play_events::event_type::basic, play_events::basic_event_type::enemy_bullet_destroyed);
}

auto level_container::OnDestroyed(const power_up& object) -> void
{
  play_events::increment(play_events::counter_type::power_ups_collected);

  switch( object.Type() )
  {
    case power_up::type::level_completion:
      play_events::add(play_events::event_type::basic, play_events::basic_event_type::power_up_collected);
      break;
    case power_up::type::time_bonus:    
      play_events::add(play_events::event_type::basic, play_events::basic_event_type::time_bonus_collected);
      break;
  }
}
