#include "pch.h"
#include "level_container.h"
#include "explosion.h"
#include "renderers.h"
#include "game_settings.h"
#include "dynamic_object_functions.h"
#include "particle_functions.h"

auto level_container::SetPlayerActive(bool value) -> void
{
  m_playerActive = value;
}

auto level_container::Update(float interval, D2D1_RECT_F viewRect) -> void
{
  auto updateStart = performance_counter::QueryValue();
  UpdateObjects(interval);

  auto collisionsStart = performance_counter::QueryValue();

  if( !m_playerShip->Destroyed() )
  {
    DoPlayerCollisions();
  }

  DoNonPlayerCollisions();

  auto collisionsEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"collisions", collisionsEnd - collisionsStart, game_settings::swapChainRefreshRate());

  RemoveDestroyedObjects();

  m_targettedObject = GetTargettedObject();

  CreateNewObjects(interval);

  auto updateEnd = performance_counter::QueryValue();
  diagnostics::addTime(L"level_container::update", updateEnd - updateStart, game_settings::swapChainRefreshRate());
}

auto level_container::UpdateObjects(float interval) -> void
{
  auto playerCollisionGeometry = m_playerShip->ShieldsUp() ? static_cast<ID2D1Geometry*>(level_geometries::PlayerShieldGeometry()) : static_cast<ID2D1Geometry*>(level_geometries::PlayerShipGeometry());
  m_playerShip.Update(playerCollisionGeometry, interval, m_playerActive);

  dynamic_object_functions::update(m_mines, level_geometries::MineGeometry(), interval, m_playerShip->Destroyed() ? std::nullopt : std::optional<D2D1_POINT_2F>(m_playerShip->Position()));
  dynamic_object_functions::update(m_targets, level_geometries::TargetGeometry(), interval);
  dynamic_object_functions::update(m_ductFans, level_geometries::DuctFanGeometry(), interval);
  dynamic_object_functions::update(m_bullets, interval);
  dynamic_object_functions::update(m_explosionParticles, interval);
  dynamic_object_functions::update(m_impactParticles, interval);
  dynamic_object_functions::update(m_thrustParticles, interval);
}

auto level_container::ValidateObjectPointers() -> void
{
  for( auto& bullet : m_bullets )
  {
    bullet.ValidateTargetPointer();
  }
}

auto level_container::RemoveDestroyedObjects() -> void
{
  ValidateObjectPointers();

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

  diagnostics::addTime(L"render", renderEnd - renderStart, game_settings::swapChainRefreshRate());
}

auto level_container::DoPlayerCollisions() -> void
{
  geometry_collision<player_ship, level_target> destroyShipOnTargetCollision { [this](auto& ship, auto& target)
  {
    ship.ApplyFatalDamage();
    m_explosions.emplace_back(ship.PreviousPosition());
  }};

  geometry_collision<player_ship, mine> destroyShipOnMineCollision { [this](auto& ship, auto& mine)
  {
    ship.ApplyFatalDamage();
    mine.Destroy();
    m_explosions.emplace_back(mine.PreviousPosition());
  }};

  particle_collision<player_ship, thrust_particle> destroythrustParticleOnCollision { [this](auto ship, auto& particle)
  {
    particle.Destroy();
  }};

  destroyShipOnTargetCollision(m_playerShip, m_targets);
  destroyShipOnMineCollision(m_playerShip, m_mines);
  DestroyObjectOnGeometryCollision<player_ship>(m_playerShip);
}

auto level_container::DoNonPlayerCollisions() -> void
{
  particle_collision<level_target, bullet> destroyBulletsAndDamageTarget { [this](auto& target, auto& bullet)
  {
    if( !target->IsActivated() )
    {
#ifdef TARGETS_ARE_TARGETS
      target->HitByBullet();
      m_activatedTargetCount += target->IsActivated() ? 1 : 0;
#endif
      m_playEvents.SetEvent(play_events::event_type::target_activated, target->IsActivated());
    }

    m_impacts.emplace_back(bullet.Position());
    bullet.Destroy();
  }};

  particle_collision<mine, bullet> destroyBulletsAndMinesOnCollision { [this](auto& mine, auto& bullet)
  {
    bullet.Destroy();

    if( mine->HardnessType() == mine::hardness_type::soft )
    {
      m_explosions.emplace_back(mine->PreviousPosition());
      mine->Destroy();
    }
  }};

  collision<mine> destroyMineOnMineCollision { [this](auto& mine1, auto& mine2)
  {
    m_explosions.emplace_back(mine1.PreviousPosition());
    m_explosions.emplace_back(mine2.PreviousPosition());
    mine1.Destroy();
    mine2.Destroy();
  }};

  destroyBulletsAndDamageTarget(m_targets, m_bullets);
  destroyBulletsAndMinesOnCollision(m_mines, m_bullets);
  destroyMineOnMineCollision(m_mines);
  DestroyObjectsOnGeometryCollision<mine>(m_mines);
  DestroyParticlesOnGeometryCollision<explosion_particle>(m_explosionParticles);
  DestroyParticlesOnGeometryCollision<thrust_particle>(m_thrustParticles);
  DestroyBulletsOnGeometryCollision(m_bullets);
}

auto level_container::CreateNewObjects(float interval) -> void
{
  if( m_targettedObject && m_playerShip->CanShoot() )
  {
    auto angleToTarget = direct2d::GetAngleBetweenPoints(m_playerShip->Position(), m_targettedObject->Position());
    m_bullets.emplace_back(m_playerShip->Position(), direct2d::CalculateVelocity(500, angleToTarget), m_targettedObject);
    m_playEvents.SetEvent(play_events::event_type::shot, true);
  }

  std::optional<D2D1_POINT_2F> playerPosition = m_playerShip->Destroyed() ? std::nullopt : std::optional<D2D1_POINT_2F>(m_playerShip->Position());

  auto shootingTargets = m_targets | std::ranges::views::filter([&playerPosition](const auto& target) -> bool { return playerPosition && target->CanShootAt(*playerPosition); });

  for( const auto& target : shootingTargets )
  {
    auto [thrust, maxSpeed, hardnessType] = m_stage.MineParameters();
    m_mines.emplace_back(level_geometries::MineGeometry(), target->Position(), thrust, maxSpeed, hardnessType);
  }

  for( const auto& position : m_explosions )
  {
    std::ranges::copy(level_explosion { position }, std::back_inserter(m_explosionParticles));
    m_playEvents.SetEvent(play_events::event_type::explosion, true);
  }

  m_explosions.clear();

  for( const auto& position : m_impacts )
  {
    m_impactParticles.emplace_back(position);
  }

  m_impacts.clear();

  if( m_playerShip->EmitThrustParticle() )
  {
    auto thrustAngle = direct2d::RotateAngle(m_playerShip->Angle(), 180);
    auto thrustPosition = direct2d::CalculatePosition(m_playerShip->Position(), thrustAngle, 20);
    auto thrustVelocity = direct2d::CombineVelocities(m_playerShip->Velocity(), direct2d::CalculateVelocity(50.0f, thrustAngle));
    m_thrustParticles.emplace_back(thrustPosition, thrustVelocity, 0.5f);
  }
}

auto level_container::GetTargettedObject() -> targetted_object_type
{
  m_targetPosition = direct2d::CalculatePosition(m_playerShip->Position(), m_playerShip->Angle(), 100);

  if( m_targetPosition )
  {
    auto targetAngle = m_playerShip->Angle();

    constexpr auto angleSpan = 20.0f;

    mine_object* nearestMine = std::accumulate(std::begin(m_mines), std::end(m_mines), static_cast<mine_object*>(nullptr), [this, targetAngle](auto* nearest, auto& next) -> mine_object*
    {
      auto mineAngle = direct2d::GetAngleBetweenPoints(m_playerShip->Position(), next->Position());
      auto angleDifference = direct2d::GetAngleDifference(targetAngle, mineAngle);
      if( angleDifference < -angleSpan || angleDifference > angleSpan ) return nearest;
      else return nearest ? &GetNearestToTarget(*nearest, next) : &next;
    });

#ifdef TARGETS_ARE_TARGETS
    auto activeTargets = std::ranges::views::filter(m_targets, [](const auto& target) { return target->IsActivated() ? false : true; });

    target_object* nearestTarget = std::accumulate(std::ranges::begin(activeTargets), std::ranges::end(activeTargets), static_cast<target_object*>(nullptr), [this, targetAngle](auto* nearest, auto& next) -> target_object*
    {
      auto mineAngle = direct2d::GetAngleBetweenPoints(m_playerShip->Position(), next->Position());
      auto angleDifference = direct2d::GetAngleDifference(targetAngle, mineAngle);
      if( angleDifference < -angleSpan || angleDifference > angleSpan ) return nearest;
      return nearest ? &GetNearestToTarget(*nearest, next) : &next;
    });
#else
  target_object* nearestTarget = nullptr;
#endif

    return GetNearestObject(nearestMine, nearestTarget, m_maxTargetRange);
  }
  else
  {
    return std::nullopt;
  }
}

auto level_container::DestroyBulletsOnGeometryCollision(std::ranges::input_range auto&& bullets) -> void
{
  auto unaryFunction = [this](auto& particle)
  {
    m_impacts.emplace_back(particle.Position());
    particle.Destroy();
  };

  auto binaryFunction =  [this](auto& geometry, auto& particle)
  {
    m_impacts.emplace_back(particle.Position());
    particle.Destroy();
  };

  particle_containment<bullet> destroyBulletsAtBoundary { unaryFunction };
  particle_collision<level_asteroid, bullet> destroyBulletsOnAsteroids { binaryFunction };
  particle_collision<duct_fan, bullet> destroyBulletsOnDuctFans { binaryFunction };

  destroyBulletsAtBoundary(m_boundary, bullets);
  destroyBulletsOnAsteroids(m_asteroids, bullets);
  destroyBulletsOnDuctFans(m_ductFans, bullets);
}
