#include "pch.h"
#include "level_container.h"
#include "bullet.h"
#include "explosion.h"

auto update_all(std::ranges::input_range auto&& objects, int64_t ticks)
{
  for( auto& object : objects )
  {
    object.Update(ticks);
  }
}

auto erase_destroyed(std::ranges::input_range auto&& objects)
{
  auto object = std::begin(objects);

  while( object != std::end(objects) )
  {
    object = object->Destroyed() ? objects.erase(object) : ++object;
  }
}

[[nodiscard]] auto have_geometry_and_point_collided(auto& geometryObject, auto& pointObject) -> bool
{
  const auto& geometry = geometryObject.Geometry();
  const auto& point = pointObject.Position();

  BOOL collision = FALSE;
  HRESULT hr = geometry.Get()->FillContainsPoint({point.x, point.y}, D2D1::Matrix3x2F::Identity(), &collision);

  return SUCCEEDED(hr) && collision ? true : false;
}

[[nodiscard]] auto have_geometries_collided(auto& geometryObject1, auto& geometryObject2) -> bool
{
  const auto& geometry1 = geometryObject1.Geometry();
  const auto& geometry2 = geometryObject2.Geometry();

  D2D1_GEOMETRY_RELATION relation = D2D1_GEOMETRY_RELATION_UNKNOWN;
  HRESULT hr = geometry1.Get()->CompareWithGeometry(geometry2.Get(), D2D1::Matrix3x2F::Identity(), &relation);

  bool collided = false;

  if( SUCCEEDED(hr) )
  {
    switch( relation )
    {
      case D2D1_GEOMETRY_RELATION_IS_CONTAINED:
      case D2D1_GEOMETRY_RELATION_CONTAINS:
      case D2D1_GEOMETRY_RELATION_OVERLAP:
        collided = true;
        break;
    }
  }

  return collided;
}

auto do_geometry_to_point_collisions(std::ranges::input_range auto&& geometryObjects, std::ranges::input_range auto&& pointObjects, auto OnCollision) -> void
{
  std::for_each(std::execution::par, std::begin(geometryObjects), std::end(geometryObjects), [&pointObjects, OnCollision](auto& geometryObject)
  {
    for( auto& pointObject : pointObjects )
    {
      {
        if( have_geometry_and_point_collided(geometryObject, pointObject) )
        {
          OnCollision(geometryObject, pointObject);
        }
      }
    }
  });
}

auto do_geometry_to_geometry_collisions(std::ranges::input_range auto&& geometryObjects1, std::ranges::input_range auto&& geometryObjects2, auto OnCollision) -> void
{
  std::for_each(std::execution::par, std::begin(geometryObjects1), std::end(geometryObjects1), [&geometryObjects2, OnCollision](auto& geometryObject1)
  {
    for( auto& geometryObject2 : geometryObjects2 )
    {
      {
        if( have_geometries_collided(geometryObject1, geometryObject2) )
        {
          OnCollision(geometryObject1, geometryObject2);
        }
      }
    }
  });
}

level_container::level_container()
{
  m_playerShips.emplace_back( player_ship {} );
}

level_container::level_container(ID2D1RenderTarget* renderTarget)
{
  m_playerShips.emplace_back( player_ship {} );
}

auto level_container::SetTimeout(int time) -> void
{
  m_ticksRemaining = performance_counter::QueryFrequency() * time;
}

auto level_container::HasTimedOut() const -> bool
{
  return m_ticksRemaining == 0;
}

[[nodiscard]] auto level_container::IsComplete() const -> bool
{
  return m_activatedTargetCount == m_targets.size();
}

[[nodiscard]] auto level_container::HasFinished() const -> bool
{
  return HasTimedOut() || PlayerDied() || IsComplete();
}

auto level_container::Update(const object_input_data& inputData, int64_t ticks, D2D1_RECT_F viewRect) -> void
{
  m_playerShot = false;
  m_targetActivated = false;

  for( auto& playerShip : m_playerShips )
  {
    auto playerPosition = playerShip.Position();
    auto playerToMouseAngle = CalculateAngle(playerPosition.x, playerPosition.y, inputData.GetMouseData().x, inputData.GetMouseData().y);
    playerShip.SetAngle(playerToMouseAngle);
    playerShip.SetThrusterOn(inputData.GetMouseData().rightButtonDown);
  }

  update_all(m_playerShips, ticks);
  update_all(m_bullets, ticks);

  auto triggerPressed = inputData.GetMouseData().leftButtonDown;

  for( auto& playerShip : m_playerShips )
  {
    auto playerPosition = playerShip.Position();
    
    if( triggerPressed && playerShip.CanShoot() )
    {
      m_bullets.emplace_back( bullet { playerPosition.x, playerPosition.y, playerShip.Angle() } );
    }

    m_background.SetCentre(playerPosition.x, playerPosition.y);
  }

  m_background.Update(ticks);

  m_asteroids.clear();

  CreateAsteroids(viewRect, std::back_inserter(m_asteroids));

  do_geometry_to_geometry_collisions(m_asteroids, m_playerShips, [](level_asteroid& asteroid, player_ship& playerShip)
  {
    playerShip.Destroy();
  });

  do_geometry_to_point_collisions(m_asteroids, m_bullets, [](level_asteroid& asteroid, bullet& bullet)
  {
    bullet.Destroy();
  });

  do_geometry_to_point_collisions(m_targets, m_bullets, [this](level_target& target, bullet& bullet)
  {
    target.Activate();
    ++m_activatedTargetCount;
    bullet.Destroy();
  });

  erase_destroyed(m_playerShips);
  erase_destroyed(m_bullets);

  m_ticksRemaining -= ticks;
  m_ticksRemaining = max(0, m_ticksRemaining);
}

auto level_container::Render(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void
{
  m_background.Render(viewRect);
  renderer::render_all(m_targets);
  renderer::render_all(m_asteroids);
  renderer::render_all(m_bullets);
  renderer::render_all(m_playerShips);
}

[[nodiscard]] auto level_container::Targets() const -> const target_collection&
{
  return m_targets;
}

[[nodiscard]] auto level_container::PlayerPosition() const -> game_point
{
  return std::reduce(std::cbegin(m_playerShips), std::end(m_playerShips), game_point { 0, 0 }, [](auto postion, const auto& playerShip) { return playerShip.Position(); });
}

[[nodiscard]] auto level_container::PlayerHasThrusterOn() const -> bool
{
  return std::reduce(std::cbegin(m_playerShips), std::end(m_playerShips), false, [](auto thrusterOn, const auto& playerShip) { return playerShip.ThrusterOn(); });
}

[[nodiscard]] auto level_container::PlayerShot() const -> bool
{
  return m_playerShot;
}

[[nodiscard]] auto level_container::PlayerDied() const -> bool
{
  return m_playerShips.size() == 0;
}

[[nodiscard]] auto level_container::TargetActivated() const -> bool
{
  return m_targetActivated;
}

[[nodiscard]] auto level_container::TicksRemaining() const -> int64_t
{
  return m_ticksRemaining;
}
