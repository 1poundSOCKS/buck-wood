#include "pch.h"
#include "level_container.h"
#include "explosion.h"
#include "game_clock.h"
#include "renderers.h"
#include "game_settings.h"

auto level_container::Update(const level_input& input, int64_t ticks, D2D1_RECT_F viewRect) -> update_events
{
  auto interval = game_clock::getInterval(ticks);

  auto updateStart = performance_counter::QueryValue();

  auto shootTarget = CalculateTargettedMine();
  auto angleToTarget = shootTarget ? std::optional<float>(m_playerShip->Position().AngleTo(shootTarget->Position())) : std::nullopt;
  auto shootAngle = input.ShootAngle() && angleToTarget ? angleToTarget : input.ShootAngle();
  
  player_ship::update_events playerUpdateEvents;
  m_playerShip.Update(interval, input.Thrust(), input.Angle(), input.Rotation(), shootAngle ? true : false, &playerUpdateEvents);

  std::optional<game_point> playerPosition = m_playerShip->Destroyed() ? std::nullopt : std::optional<game_point>(m_playerShip->Position());
  UpdateObjects(interval, playerPosition);

  auto updateEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"update", updateEnd - updateStart, game_settings::framerate());

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

  EraseDestroyedObjects();

  if( !m_playerShip->Destroyed() && shootAngle )
  {
    m_bullets.Create(m_playerShip->Position(), m_playerShip->Velocity(), *shootAngle);
  }

  CreateNewObjects(interval, playerPosition);

  m_activatedTargetCount += m_collisionChecks.TargetActivationCount();

  return update_events { shootAngle ? true : false, m_collisionChecks.TargetActivationCount() ? true : false, 
    m_collisionChecks.Explosions().size() || m_containmentChecks.Explosions().size() ? true : false };
}

auto level_container::UpdateObjects(float interval, const std::optional<game_point>& playerPosition) -> void
{
  m_mines.Update(interval, playerPosition);
  m_targets.Update(interval);
  m_ductFans.Update(interval);
  m_bullets.Update(interval);
  m_explosionParticles.Update(interval);
  m_impactParticles.Update(interval);
  m_thrustParticles.Update(interval);  
}

auto level_container::EraseDestroyedObjects() -> void
{
  m_bullets.EraseDestroyed();
  m_explosionParticles.EraseDestroyed();
  m_impactParticles.EraseDestroyed();
  m_thrustParticles.EraseDestroyed();
  m_mines.EraseDestroyed();
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
  m_containmentChecks.shipContainment(m_boundary.Geometry().Get(), m_playerShip);

  m_collisionChecks.shipToAsteroidCollision(m_playerShip, m_asteroids);
  m_collisionChecks.shipToTargetCollision(m_playerShip, m_targets);
  m_collisionChecks.shipToDuctFanCollision(m_playerShip, m_ductFans);
  m_collisionChecks.shipToMineCollision(m_playerShip, m_mines);
  m_collisionChecks.shipToExplosionCollision(m_playerShip, m_explosionParticles);
}

auto level_container::DoNonPlayerCollisions() -> void
{
  m_containmentChecks.mineContainment(m_boundary.Geometry().Get(), m_mines);
  m_containmentChecks.explosionContainment(m_boundary.Geometry().Get(), m_explosionParticles);
  m_containmentChecks.thrustContainment(m_boundary.Geometry().Get(), m_thrustParticles);
  m_containmentChecks.bulletContainment(m_boundary.Geometry().Get(), m_bullets);

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

auto level_container::CreateNewObjects(float interval, const std::optional<game_point>& playerPosition) -> void
{
  auto shootingTargets = m_targets | std::ranges::views::filter([&playerPosition](const auto& target) -> bool { return playerPosition && target->CanShootAt(*playerPosition); });

  for( const auto& target : shootingTargets )
  {
    m_mines.Create(level_geometries::MineGeometry(), target->Position().x, target->Position().y);
  }

  for( const auto& position : m_containmentChecks.Explosions() )
  {
    m_explosionParticles.Create( level_explosion { position } );
  }

  for( const auto& position : m_collisionChecks.Explosions() )
  {
    m_explosionParticles.Create( level_explosion { position } );
  }

  for( const auto& position : m_containmentChecks.Impacts() )
  {
    m_impactParticles.Create(position);
  }

  for( const auto& position : m_collisionChecks.Impacts() )
  {
    m_impactParticles.Create(position);
  }

  if( m_playerShip->ThrusterOn() && m_thrustEmmisionTimer.Update(interval) )
  {
    auto thrustPosition = m_playerShip->RelativePosition(180, 0, -15);
    auto thrustAngle = m_playerShip->Angle() + 180;
    auto thrustVelocity = m_playerShip->RelativeVelocity(thrustAngle, 100);
    m_thrustParticles.Create(thrustPosition, thrustVelocity, 0.3f);
  }
}

auto level_container::CalculateTargettedMine() const -> std::optional<mine>
{
  std::optional<mine> nearestMine = std::accumulate(std::begin(m_mines), std::end(m_mines), std::optional<mine>(), [this](const auto& nearest, const auto& nextMine)
  {
    return nearest ? GetNearest(*nearest, nextMine.Object()) : nextMine.Object();
  });

  return nearestMine;
}

auto level_container::GetNearest(const mine& mine1, const mine& mine2) const -> const mine&
{
  auto playerPosition = m_playerShip.Object().Position();
  auto mine1Distance = playerPosition.DistanceTo(mine1.Position());
  auto mine2Distance = playerPosition.DistanceTo(mine2.Position());
  return mine2Distance < mine1Distance ? mine2 : mine1;
}
