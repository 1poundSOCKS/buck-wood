#include "pch.h"
#include "level_container.h"
#include "explosion.h"
#include "renderers.h"
#include "game_settings.h"
#include "dynamic_object_functions.h"
#include "particle_functions.h"

auto level_container::Update(float interval, D2D1_RECT_F viewRect) -> void
{
  auto updateStart = performance_counter::QueryValue();
  UpdateObjects(interval);

  m_mineContainmentResults.Clear();
  m_shipContainmentResults.Clear();

  m_shipToAsteroidCollisionResults.Clear();
  m_shipToTargetCollisionResults.Clear();
  m_shipToDuctFanCollisionResults.Clear();
  m_shipToMineCollisionResults.Clear();

  m_shipToExplosionCollisionResults.Clear();

  m_mineToAsteroidCollisionResults.Clear();
  m_mineToDuctFanCollisionResults.Clear();

  m_mineToBulletCollisionResults.Clear();
  m_ductFanToBulletCollisionResults.Clear();
  m_targetToBulletCollisionResults.Clear();

  m_explosions.clear();
  m_impacts.clear();

  auto collisionsStart = performance_counter::QueryValue();

  if( !m_playerShip->Destroyed() )
  {
    DoPlayerCollisions();
  }

  DoNonPlayerCollisions();

  m_maxCollisionCount = GetMaxCollisionCount(m_maxCollisionCount);
  diagnostics::add(L"max collision count", std::format(L"{}", m_maxCollisionCount));

  ProcessCollisionResults();

  auto collisionsEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"collisions", collisionsEnd - collisionsStart, game_settings::framerate());

  RemoveDestroyedObjects();

  m_targettedObject = GetTargettedObject();

  CreateNewObjects(interval);

  auto updateEnd = performance_counter::QueryValue();
  diagnostics::addTime(L"level_container::update", updateEnd - updateStart, game_settings::framerate());
}

auto level_container::UpdateObjects(float interval) -> void
{
  m_playerShip.Update(interval);
  m_playerReloadCounter.Update(interval);
  m_thrustEmmisionCounter.Update(interval);
  dynamic_object_functions::update(m_mines, interval, m_playerShip->Destroyed() ? std::nullopt : std::optional<D2D1_POINT_2F>(m_playerShip->Position()));
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
  m_shipContainmentResults.Fetch(m_boundary, m_playerShip);

  m_shipToAsteroidCollisionResults.Fetch(m_playerShip, m_asteroids);
  m_shipToTargetCollisionResults.Fetch(m_playerShip, m_targets);
  m_shipToDuctFanCollisionResults.Fetch(m_playerShip, m_ductFans);
  m_shipToMineCollisionResults.Fetch(m_playerShip, m_mines);

  m_shipToExplosionCollisionResults.Fetch(m_playerShip, m_explosionParticles);
}

auto level_container::DoNonPlayerCollisions() -> void
{
  m_mineContainmentResults.Fetch(m_boundary, m_mines);

  m_mineToAsteroidCollisionResults.Fetch(m_mines, m_asteroids);
  m_mineToDuctFanCollisionResults.Fetch(m_mines, m_ductFans);

  m_mineToBulletCollisionResults.Fetch(m_mines, m_bullets);
  m_ductFanToBulletCollisionResults.Fetch(m_ductFans, m_bullets);
  m_targetToBulletCollisionResults.Fetch(m_targets, m_bullets);
}

auto level_container::ProcessCollisionResults() -> void
{
  m_mineContainmentResults.Process([this](auto& mine)
  {
    m_explosions.emplace_back(mine.PreviousPosition());
    mine.Destroy();
  });

  m_shipContainmentResults.Process([this](auto& ship)
  {
    m_explosions.emplace_back(ship.PreviousPosition());
    ship.ApplyFatalDamage();
  });

  m_shipToAsteroidCollisionResults.Process([this](auto& playerShip, auto& asteroid)
  {
    playerShip.ApplyFatalDamage();
    m_explosions.emplace_back(playerShip.PreviousPosition());
  });

  m_shipToTargetCollisionResults.Process([this](auto& playerShip, auto& target)
  {
    playerShip.ApplyFatalDamage();
    m_explosions.emplace_back(playerShip.PreviousPosition());
  });

  m_shipToDuctFanCollisionResults.Process([this](auto& playerShip, auto& ductFan)
  {
    playerShip.ApplyFatalDamage();
    auto position = playerShip.PreviousPosition();
    m_explosions.emplace_back(playerShip.PreviousPosition());
  });

  m_shipToMineCollisionResults.Process([this](auto& playerShip, auto& mine)
  {
    playerShip.ApplyDamage(2);
    mine.Destroy();
    m_explosions.emplace_back(mine.PreviousPosition());
  });

  m_shipToExplosionCollisionResults.Process([](auto& object, auto& particle)
  {
    particle.Destroy();
  });

  m_mineToAsteroidCollisionResults.Process([this](auto& mine, auto& asteroid)
  {
    auto position = mine.PreviousPosition();
    mine.Destroy();
    m_explosions.emplace_back(mine.PreviousPosition());
  });

  m_mineToDuctFanCollisionResults.Process([this](auto& mine, auto& ductFan)
  {
    auto position = mine.PreviousPosition();
    mine.Destroy();
    m_explosions.emplace_back(mine.PreviousPosition());
  });

  m_mineToBulletCollisionResults.Process([this](auto& object, auto& particle)
  {
    object.Destroy();
    particle.Destroy();
    m_explosions.emplace_back(object.Position());
  });

  m_ductFanToBulletCollisionResults.Process([this](auto& object, auto& particle)
  {
    particle.Destroy();
    m_impacts.emplace_back(object.Position());
  });

  m_targetToBulletCollisionResults.Process([this](auto& object, auto& particle)
  {
    if( !object.IsActivated() )
    {
      object.HitByBullet();
      m_activatedTargetCount += object.IsActivated() ? 1 : 0;
    }

    m_impacts.emplace_back(particle.Position());
    particle.Destroy();
  });

  m_destroyBulletsAtBoundary(m_boundary, m_bullets);
  m_destroyBulletsOnAsteroids(m_asteroids, m_bullets);

  DestroyParticlesOnGeometryCollision<explosion_particle>(m_explosionParticles);
  DestroyParticlesOnGeometryCollision<thrust_particle>(m_thrustParticles);
}

auto level_container::CreateNewObjects(float interval) -> void
{
  if( m_targettedObject && gamepad_reader::right_trigger() > 0 && m_playerReloadCounter.Get(1) == 1 )
  {
    auto angleToTarget = direct2d::GetAngleBetween(m_playerShip->Position(), m_targettedObject->Position());
    m_bullets.emplace_back(m_playerShip->Position(), game_velocity { angleToTarget, 500.0f }, m_targettedObject);
    m_playEvents.SetEvent(play_events::event_type::shot, true);
  }

  std::optional<D2D1_POINT_2F> playerPosition = m_playerShip->Destroyed() ? std::nullopt : std::optional<D2D1_POINT_2F>(m_playerShip->Position());

  auto shootingTargets = m_targets | std::ranges::views::filter([&playerPosition](const auto& target) -> bool { return playerPosition && target->CanShootAt(*playerPosition); });

  for( const auto& target : shootingTargets )
  {
    m_mines.emplace_back(level_geometries::MineGeometry(), target->Position());
  }

  for( const auto& position : m_explosions )
  {
    std::ranges::copy(level_explosion { position }, std::back_inserter(m_explosionParticles));
    m_playEvents.SetEvent(play_events::event_type::explosion, true);
  }

  for( const auto& position : m_impacts )
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

auto level_container::GetMaxCollisionCount(int currentMaxCollisionCount) const -> int
{
  return std::max(currentMaxCollisionCount, 
    static_cast<int>(m_shipToAsteroidCollisionResults.Count() + 
      m_shipToTargetCollisionResults.Count() +
      m_shipToDuctFanCollisionResults.Count() +
      m_shipToMineCollisionResults.Count() +
      m_shipToExplosionCollisionResults.Count() +
      m_mineToAsteroidCollisionResults.Count() +
      m_mineToDuctFanCollisionResults.Count() +
      m_mineToBulletCollisionResults.Count() +
      m_ductFanToBulletCollisionResults.Count() +
      m_targetToBulletCollisionResults.Count()));
}
