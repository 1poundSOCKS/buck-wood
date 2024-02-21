#include "pch.h"
#include "level_container.h"
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

  DoCollisions();

  auto collisionsEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"collisions", collisionsEnd - collisionsStart, game_settings::swapChainRefreshRate());

  RemoveDestroyedObjects();

  m_targettedObject = GetTargettedObject();

  // CreateNewObjects(interval);
  CreateNewObjects2(interval);

  auto updateEnd = performance_counter::QueryValue();
  diagnostics::addTime(L"level_container::update", updateEnd - updateStart, game_settings::swapChainRefreshRate());
}

auto level_container::UpdateObjects(float interval) -> void
{
  // auto playerCollisionGeometry = m_playerShip->ShieldsUp() ? static_cast<ID2D1Geometry*>(level_geometries::PlayerShieldGeometry()) : static_cast<ID2D1Geometry*>(level_geometries::PlayerShipGeometry());
  // m_playerShip.Update(playerCollisionGeometry, interval, m_playerActive);

  // dynamic_object_functions::update(m_mines, level_geometries::MineGeometry(), interval, m_playerShip->Destroyed() ? std::nullopt : std::optional<D2D1_POINT_2F>(m_playerShip->Position()));
  // dynamic_object_functions::update(m_targets, level_geometries::TargetGeometry(), interval);
  // dynamic_object_functions::update(m_ductFans, level_geometries::DuctFanGeometry(), interval);
  dynamic_object_functions::update(m_bullets, interval);
  dynamic_object_functions::update(m_particles, interval);

  // dynamic_object_functions::update(m_defaultObjects, level_geometries::TargetGeometry(), interval);

  struct visitor
  {
    level_container& m_levelContainer;
    dynamic_object<default_object>& m_dynamicObject;
    float m_interval;

    auto operator()(const level_target& object)
    {
      m_dynamicObject.Update(level_geometries::TargetGeometry(), m_interval);
    }
    auto operator()(const player_ship& object)
    {
      m_dynamicObject.Update(level_geometries::PlayerShipGeometry(), m_interval);
      m_levelContainer.SetPlayerPosition(object.Position());
    }
    auto operator()(const mine& object)
    {
      m_dynamicObject.Update(level_geometries::MineGeometry(), m_interval);
    }
  };

  for( auto& object : m_staticObjects )
  {
    std::visit(visitor { *this, object, interval }, object->Get());
  }

  for( auto& object : m_movingObjects )
  {
    std::visit(visitor { *this, object, interval }, object->Get());
  }
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

  // dynamic_object_functions::erase_destroyed(m_mines);
  particle_functions::erase_destroyed(m_bullets);
  particle_functions::erase_destroyed(m_particles);

  dynamic_object_functions::erase_destroyed(m_movingObjects);
}

auto level_container::Render(D2D1_RECT_F viewRect) const -> void
{
  auto renderStart = performance_counter::QueryValue();

  renderer::render(m_boundary);
  // renderer::render_all(m_asteroids);
  // renderer::render_all(m_mines);
  // renderer::render_all(m_targets);
  // renderer::render_all(m_ductFans);

  renderer::render_all(m_staticObjects);
  renderer::render_all(m_movingObjects);

  renderer::render_all(m_bullets);
  renderer::render_all(m_particles);

  // if( !m_playerShip->Destroyed() )
  // {
  //   renderer::render(m_playerShip);
  // }

  auto renderEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"render", renderEnd - renderStart, game_settings::swapChainRefreshRate());
}

auto level_container::DoCollisions() -> void
{
  level_collision_handler<level_container> collisionHandler { *this };
  
  // if( !m_playerShip->Destroyed() )
  // {
  //   geometry_collision<player_ship, level_target> shipOnTargetCollision { collisionHandler };
  //   shipOnTargetCollision(m_playerShip, m_targets);

  //   geometry_collision<player_ship, mine> shipOnMineCollision { collisionHandler };
  //   shipOnMineCollision(m_playerShip, m_mines);

  //   collisionHandler.DestroyObjectOnGeometryCollision<player_ship>(m_playerShip, m_boundary, m_asteroids, m_ductFans);
  // }

  // particle_collision<level_target, bullet> bulletOnTargetCollision { collisionHandler };
  // bulletOnTargetCollision(m_targets, m_bullets);

  // particle_collision<mine, bullet> bulletOnMineCollision { collisionHandler };
  // bulletOnMineCollision(m_mines, m_bullets);

  // collision<mine> mineOnMineCollision { collisionHandler };
  // mineOnMineCollision(m_mines);

  // collisionHandler.DestroyObjectsOnGeometryCollision<mine>(m_mines, m_boundary, m_asteroids, m_ductFans);
  // collisionHandler.DestroyParticlesOnGeometryCollision<particle>(m_particles, m_boundary, m_asteroids, m_ductFans);
  // collisionHandler.DestroyBulletsOnGeometryCollision(m_bullets, m_boundary, m_asteroids, m_ductFans);

  geometry_collision<default_object, default_object> collisionRunner { collisionHandler };
  collisionRunner(m_staticObjects, m_movingObjects);
}

auto level_container::CreateNewObjects(float interval) -> void
{
  auto damageMode = ConvertFireModeToDamageMode(m_playerShip->FireMode());
  
  if( m_targettedObject && m_playerShip->CanShoot() && damageMode )
  {
    auto angleToTarget = direct2d::GetAngleBetweenPoints(m_playerShip->Position(), m_targettedObject->Position());
    m_bullets.emplace_back(m_playerShip->Position(), direct2d::CalculateVelocity(500, angleToTarget), *damageMode, m_targettedObject);
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
    std::ranges::copy(level_explosion { position }, std::back_inserter(m_particles));
    m_playEvents.SetEvent(play_events::event_type::explosion, true);
  }

  m_explosions.clear();

  for( const auto& position : m_impacts )
  {
    m_particles.emplace_back(particle::type::impact, position, VELOCITY_2F { 0, 0 }, 0.5f);
  }

  m_impacts.clear();

  if( m_playerShip->EmitThrustParticle() )
  {
    auto thrustAngle = direct2d::RotateAngle(m_playerShip->Angle(), 180);
    auto thrustPosition = direct2d::CalculatePosition(m_playerShip->Position(), thrustAngle, 20);
    auto thrustVelocity = direct2d::CombineVelocities(m_playerShip->Velocity(), direct2d::CalculateVelocity(50.0f, thrustAngle));
    m_particles.emplace_back(particle::type::thrust, thrustPosition, thrustVelocity, 0.5f);
  }
}

auto level_container::CreateNewObjects2(float interval) -> void
{
  for( const auto& position : m_explosions )
  {
    std::ranges::copy(level_explosion { position }, std::back_inserter(m_particles));
    m_playEvents.SetEvent(play_events::event_type::explosion, true);
  }

  m_explosions.clear();

  struct visitor
  {
    level_container& m_levelContainer;

    auto operator()(level_target& object)
    {
      if( m_levelContainer.PlayerPosition() && object.CanShootAt(*m_levelContainer.PlayerPosition()) )
      {
        auto [thrust, maxSpeed, hardnessType] = m_levelContainer.MineParameters();
        m_levelContainer.CreateMovingObject(level_geometries::MineGeometry(), std::in_place_type<mine>, object.Position(), thrust, maxSpeed, hardnessType);
      }
    }

    auto operator()(player_ship& object)
    {
      auto damageMode = ConvertFireModeToDamageMode(object.FireMode());
      
      if( m_levelContainer.TargettedObject() && object.CanShoot() && damageMode )
      {
        auto angleToTarget = direct2d::GetAngleBetweenPoints(object.Position(), m_levelContainer.TargettedObject()->Position());
        m_levelContainer.CreateBullet(object.Position(), direct2d::CalculateVelocity(500, angleToTarget), *damageMode, std::nullopt);
        m_levelContainer.SetPlayEvent(play_events::event_type::shot, true);
      }

      if( object.EmitThrustParticle() )
      {
        auto thrustAngle = direct2d::RotateAngle(object.Angle(), 180);
        auto thrustPosition = direct2d::CalculatePosition(object.Position(), thrustAngle, 20);
        auto thrustVelocity = direct2d::CombineVelocities(object.Velocity(), direct2d::CalculateVelocity(50.0f, thrustAngle));
        m_levelContainer.CreateParticle(particle::type::thrust, thrustPosition, thrustVelocity, 0.5f);
      }
    }

    auto operator()(mine& object)
    {
    }
  };

  for( auto& object : m_staticObjects )
  {
    std::visit(visitor { *this }, object.Object().Get());
  }

  for( auto& object : m_movingObjects )
  {
    std::visit(visitor { *this }, object.Object().Get());
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
