#include "pch.h"
#include "explosion.h"
#include "render_brush_defs.h"

std::uniform_int_distribution<int> particleAngleDist(0, 359);
std::uniform_int_distribution<int> particleSpeedDist(200, 300);

explosion_state::particle_state::particle_state()
{
}

auto explosion_state::particle_state::Initialize(float x, float y) -> void
{
  m_x = x;
  m_y = y;

  auto angle = static_cast<float>(particleAngleDist(framework::rng()));
  const auto velocity = static_cast<float>(particleSpeedDist(framework::rng()));

  m_xVelocity = CalculateVectorX(velocity, angle);
  m_yVelocity = CalculateVectorY(velocity, angle);
}

auto explosion_state::particle_state::Update(float interval) -> void
{
  m_x += m_xVelocity * interval;
  m_y += m_yVelocity * interval;
}

auto explosion_state::particle_state::HasCollidedWith(const collision_data& collisionData) const -> bool
{
  return collisionData.PointInside(m_x, m_y);
}

auto explosion_state::particle_state::GetRenderRect(ID2D1SolidColorBrush* brush) const -> render_rect
{
  const auto particleSize = 6.0f;
  return { m_x - particleSize, m_y - particleSize, m_x + particleSize, m_y + particleSize, brush };
}

explosion_state::explosion_state(float x, float y)
{
  const int particleCount = 50;
  m_particles.resize(particleCount);
  
  for( auto& particle : m_particles )
  {
    particle.Initialize(x, y);
  }

  Initialize(framework::renderTarget().get());
}

auto explosion_state::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_brush = screen_render_brush_white.CreateBrush(renderTarget);
}

auto explosion_state::Update(const object_input_data& inputData, int64_t tickCount) -> void
{
  auto updateInterval = framework::gameUpdateInterval(tickCount);

  for( auto& particle : m_particles )
  {
    particle.Update(updateInterval);
  }
}

auto explosion_state::Render(D2D1_RECT_F viewRect) const -> void
{
  std::vector<render_point> renderParticles;

  std::transform(m_particles.cbegin(), m_particles.cend(), std::back_inserter(renderParticles), [this](auto particle) -> render_rect
  {
    return particle.GetRenderRect(this->m_brush.get());
  });

  RenderPoints(framework::renderTarget().get(), renderParticles.cbegin(), renderParticles.cend());
}

[[nodiscard]] auto explosion_state::GetCollisionData() const -> const collision_data&
{
  return m_collisionData;
}

[[nodiscard]] auto explosion_state::HasCollidedWith(const collision_data& collisionData) const -> bool
{
  return false;
}

[[nodiscard]] auto explosion_state::GetCollisionEffect() const -> collision_effect
{
  return {};
}

auto explosion_state::ApplyCollisionEffect(const collision_effect& collisionEffect) -> void
{
}

[[nodiscard]] auto explosion_state::Destroyed() const -> bool
{
  return m_particles.size() == 0;
}
