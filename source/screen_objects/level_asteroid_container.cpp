#include "pch.h"
#include "level_asteroid_container.h"

level_asteroid_container::level_asteroid_container()
{
}

auto level_asteroid_container::AddAsteroid(const level_asteroid&& asteroid) -> void
{
  m_asteroids.emplace_back(asteroid);
  m_collisionData.Add(asteroid.GetCollisionData());
}

auto level_asteroid_container::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  for( auto& asteroid : m_asteroids )
  {
    asteroid.Initialize(renderTarget);
  }
}

auto level_asteroid_container::Update(const object_input_data& inputData, int64_t tickCount) -> void
{
  for( auto& asteroid : m_asteroids )
  {
    asteroid.Update(inputData, tickCount);
  }
}

auto level_asteroid_container::Render(D2D1_RECT_F viewRect) const -> void
{
  for( auto& asteroid : m_asteroids )
  {
    asteroid.Render(viewRect);
  }
}

[[nodiscard]] auto level_asteroid_container::GetCollisionData() const -> const collision_data&
{
  return m_collisionData;
}

[[nodiscard]] auto level_asteroid_container::HasCollidedWith(const collision_data& collisionData) const -> bool
{
  return false;
}

[[nodiscard]] auto level_asteroid_container::GetCollisionEffect() const -> collision_effect
{
  return m_collisionEffect;
}

auto level_asteroid_container::ApplyCollisionEffect(const collision_effect& effect) -> void
{
}

[[nodiscard]] auto level_asteroid_container::Destroyed() const -> bool
{
  return false;
}
