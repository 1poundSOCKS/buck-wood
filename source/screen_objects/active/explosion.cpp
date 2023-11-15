#include "pch.h"
#include "explosion.h"

std::uniform_int_distribution<int> particleAngleDist(0, 359);
std::uniform_int_distribution<int> particleSpeedDist(200, 300);

explosion::particle_state::particle_state()
{
}

auto explosion::particle_state::Initialize(float x, float y) -> void
{
  m_startX = m_x = x;
  m_startY = m_y = y;

  auto angle = static_cast<float>(particleAngleDist(render_target::rng()));
  const auto velocity = static_cast<float>(particleSpeedDist(render_target::rng()));

  m_xVelocity = CalculateVectorX(velocity, angle);
  m_yVelocity = CalculateVectorY(velocity, angle);
}

auto explosion::particle_state::Update(float interval) -> void
{
  m_x += m_xVelocity * interval;
  m_y += m_yVelocity * interval;
}

auto explosion::particle_state::GetRenderRect(ID2D1SolidColorBrush* brush) const -> render_rect
{
  const auto particleSize = 6.0f;
  return { m_x - particleSize, m_y - particleSize, m_x + particleSize, m_y + particleSize, brush };
}

[[nodiscard]] auto explosion::particle_state::DistanceTravelled() const -> float
{
  float cx = m_x - m_startX;
  float cy = m_y - m_startY;
  return sqrt(cx * cx + cy * cy);
}

[[nodiscard]] auto explosion::particle_state::Range() const -> float
{
  return m_range;
}

[[nodiscard]] auto explosion::particle_state::HasExpired() const -> bool
{
  return DistanceTravelled() > m_range;
}

explosion::explosion(float x, float y)
{
  const int particleCount = 50;
  m_particles.resize(particleCount);
  
  for( auto& particle : m_particles )
  {
    particle.Initialize(x, y);
  }
}

[[nodiscard]] auto explosion::Particles() const -> const particle_collection&
{
  return m_particles;
}

auto explosion::Update(int64_t tickCount) -> void
{
  auto updateInterval = render_target::gameUpdateInterval(tickCount);

  auto particle = std::begin(m_particles);

  while( particle != std::end(m_particles) )
  {
    particle->Update(updateInterval);
    particle = particle->HasExpired() ? m_particles.erase(particle) : ++particle;
  }
}

[[nodiscard]] auto explosion::Destroyed() const -> bool
{
  return m_particles.size() == 0;
}
