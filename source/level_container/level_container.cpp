#include "pch.h"
#include "level_container.h"
#include "explosion.h"
#include "game_clock.h"
#include "renderers.h"
#include "game_settings.h"
#include "dynamic_object_functions.h"
#include "particle_functions.h"

auto level_container::Update(int64_t ticks, D2D1_RECT_F viewRect) -> void
{
  auto interval = game_clock::getInterval(ticks);

  auto updateStart = performance_counter::QueryValue();
  UpdateObjects(interval);

  m_collisionChecks.Reset();
  m_containmentChecks.Reset();

  auto collisionsStart = performance_counter::QueryValue();

  if( !m_playerShip->Destroyed() )
  {
    DoPlayerCollisions();
  }

  DoNonPlayerCollisions();

  auto collisionsEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"collisions", collisionsEnd - collisionsStart, game_settings::framerate());

  RemoveDestroyedObjects();

  m_targettedObject = GetTargettedObject();

  CreateNewObjects(interval);

  m_activatedTargetCount += m_collisionChecks.TargetActivationCount();

  auto updateEnd = performance_counter::QueryValue();
  diagnostics::addTime(L"level_container::update", updateEnd - updateStart, game_settings::framerate());
}

auto level_container::UpdateObjects(float interval) -> void
{
  m_playerShip.Update(interval);
  m_playerReloadCounter.Update(interval);
  m_thrustEmmisionCounter.Update(interval);
  dynamic_object_functions::update(m_mines, interval, m_playerShip->Destroyed() ? std::nullopt : std::optional<game_point>(m_playerShip->Position()));
  dynamic_object_functions::update(m_targets, interval);
  dynamic_object_functions::update(m_ductFans, interval);
  dynamic_object_functions::update(m_bullets, interval);
  dynamic_object_functions::update(m_explosionParticles, interval);
  dynamic_object_functions::update(m_impactParticles, interval);
  dynamic_object_functions::update(m_thrustParticles, interval);
}

auto level_container::ValidateObjects() -> void
{
  for( auto& bullet : m_bullets )
  {
    bullet.ValidateTarget();
  }
}

auto level_container::RemoveDestroyedObjects() -> void
{
  ValidateObjects();

  dynamic_object_functions::erase_destroyed(m_mines);
  particle_functions::erase_destroyed(m_bullets);
  particle_functions::erase_destroyed(m_explosionParticles);
  particle_functions::erase_destroyed(m_impactParticles);
  particle_functions::erase_destroyed(m_thrustParticles);
}

auto level_container::Render(D2D1_RECT_F viewRect) const -> void
{
  auto renderStart = performance_counter::QueryValue();

  renderer::render(m_boundary);
  renderer::render_all(m_explosionParticles);
  renderer::render_all(m_asteroids);
  renderer::render_all(m_mines);
  renderer::render_all(m_targets);
  renderer::render_all(m_ductFans);
  renderer::render_all(m_impactParticles);
  renderer::render_all(m_bullets);

  if( !m_playerShip->Destroyed() )
  {
    renderer::render(m_playerShip);
  }

  renderer::render_all(m_thrustParticles);

  auto renderEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"render", renderEnd - renderStart, game_settings::framerate());
}

auto level_container::DoPlayerCollisions() -> void
{
  m_containmentChecks.shipContainment(m_boundary, m_playerShip);
  m_collisionChecks.shipToAsteroidCollision(m_playerShip, m_asteroids);
  m_collisionChecks.shipToTargetCollision(m_playerShip, m_targets);
  m_collisionChecks.shipToDuctFanCollision(m_playerShip, m_ductFans);
  m_collisionChecks.shipToMineCollision(m_playerShip, m_mines);
  m_collisionChecks.shipToExplosionCollision(m_playerShip, m_explosionParticles);
}

auto level_container::DoNonPlayerCollisions() -> void
{
  m_containmentChecks.mineContainment(m_boundary, m_mines);
  m_containmentChecks.explosionContainment(m_boundary, m_explosionParticles);
  m_containmentChecks.thrustContainment(m_boundary, m_thrustParticles);
  m_containmentChecks.bulletContainment(m_boundary, m_bullets);
  m_collisionChecks.mineToAsteroidCollision(m_mines, m_asteroids);
  m_collisionChecks.mineToDuctFanCollision(m_mines, m_ductFans);
  m_collisionChecks.mineToBulletCollision(m_mines, m_bullets);
  m_collisionChecks.asteroidToBulletCollision(m_asteroids, m_bullets);
  m_collisionChecks.asteroidToExplosionCollision(m_asteroids, m_explosionParticles);
  m_collisionChecks.asteroidToThrustCollision(m_asteroids, m_thrustParticles);
  m_collisionChecks.ductFanToBulletCollision(m_ductFans, m_bullets);
  m_collisionChecks.ductFanToExplosionCollision(m_ductFans, m_explosionParticles);
  m_collisionChecks.ductFanToThrustCollision(m_ductFans, m_thrustParticles);
  m_collisionChecks.targetToBulletCollision(m_targets, m_bullets);
}

auto level_container::CreateNewObjects(float interval) -> void
{
  if( m_targettedObject && gamepad_reader::right_trigger() > 0 && m_playerReloadCounter.Get(1) == 1 )
  {
    auto angleToTarget = m_playerShip->Position().AngleTo(m_targettedObject->Position());
    m_bullets.emplace_back(m_playerShip->Position(), game_velocity { angleToTarget, 500.0f }, m_targettedObject);
    m_playEvents.SetEvent(play_events::event_type::shot, true);
  }

  std::optional<game_point> playerPosition = m_playerShip->Destroyed() ? std::nullopt : std::optional<game_point>(m_playerShip->Position());

  auto shootingTargets = m_targets | std::ranges::views::filter([&playerPosition](const auto& target) -> bool { return playerPosition && target->CanShootAt(*playerPosition); });

  for( const auto& target : shootingTargets )
  {
    m_mines.emplace_back(level_geometries::MineGeometry(), target->Position());
  }

  for( const auto& position : m_containmentChecks.Explosions() )
  {
    std::ranges::copy(level_explosion { position }, std::back_inserter(m_explosionParticles));
    m_playEvents.SetEvent(play_events::event_type::explosion, true);
  }

  for( const auto& position : m_collisionChecks.Explosions() )
  {
    std::ranges::copy(level_explosion { position }, std::back_inserter(m_explosionParticles));
    m_playEvents.SetEvent(play_events::event_type::explosion, true);
  }

  for( const auto& position : m_containmentChecks.Impacts() )
  {
    m_impactParticles.emplace_back(position);
  }

  for( const auto& position : m_collisionChecks.Impacts() )
  {
    m_impactParticles.emplace_back(position);
  }

  if( m_playerShip->ThrusterOn() && m_thrustEmmisionCounter.Get(1) == 1 )
  {
    auto thrustPosition = m_playerShip->RelativePosition(180, 0, -15);
    auto thrustAngle = m_playerShip->Angle() + 180;
    auto thrustVelocity = m_playerShip->RelativeVelocity(thrustAngle, 100);
    m_thrustParticles.emplace_back(thrustPosition, thrustVelocity, 0.3f);
  }
}

auto level_container::GetTargettedObject() -> targetted_object_type
{
  mine_object* nearestMine = std::accumulate(std::begin(m_mines), std::end(m_mines), static_cast<mine_object*>(nullptr), [this](auto* nearest, auto& next) -> mine_object*
  {
    return nearest ? &GetNearestToPlayer(*nearest, next) : &next;
  });

  auto activeTargets = std::ranges::views::filter(m_targets, [](const auto& target) { return target->IsActivated() ? false : true; });

  target_object* nearestTarget = std::accumulate(std::ranges::begin(activeTargets), std::ranges::end(activeTargets), static_cast<target_object*>(nullptr), [this](auto* nearest, auto& next) -> target_object*
  {
    return nearest ? &GetNearestToPlayer(*nearest, next) : &next;
  });

  return GetNearestObject(nearestMine, nearestTarget, 500.0f);
}
