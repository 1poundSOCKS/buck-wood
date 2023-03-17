#include "pch.h"
#include "explosion.h"

std::uniform_int_distribution<int> particleAngleDist(0, 359);
std::uniform_int_distribution<int> particleSpeedDist(150, 200);

void SetParticleVelocity(particle_state& particleState, float angle);

explosion_state::explosion_state(screen_render_brush_selector brushes)
{
  brush.attach(brushes[white]);
  brush->AddRef();
}

auto explosion_state::Update(int64_t tickFrequency, int64_t tickCount, play_event_inserter playEventInserter) -> void
{
  const auto gameSpeedMultiplier = 2.0f;
  const auto forceOfGravity = 20.0f;

  auto updateInterval = static_cast<float>(tickCount) / static_cast<float>(tickFrequency) * gameSpeedMultiplier;

  for( auto& particle : particles )
  {
    if( particle.condition == particle_state::alive )
    {
      particle.yVelocity += forceOfGravity * updateInterval;
      particle.x += particle.xVelocity * updateInterval;
      particle.y += particle.yVelocity * updateInterval;
    }
  }
}

[[nodiscard]] auto explosion_state::HasCollided(float x, float y) const -> bool
{
  return false;
}

auto explosion_state::HitByBullet() -> void
{
}

[[nodiscard]] auto explosion_state::LevelIsComplete() const -> bool
{
  return true;
}

void explosion_state::RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const
{
  std::vector<render_point> explosionParticles;
  std::vector<particle_state> aliveParticles;

  std::copy_if(particles.cbegin(), particles.cend(), std::back_inserter(aliveParticles), [](auto particle) -> bool
  {
    return particle.condition == particle_state::alive;
  });

  std::transform(aliveParticles.cbegin(), aliveParticles.cend(), std::back_inserter(explosionParticles), [this](auto state) -> render_rect
  {
    const auto particleSize = 6.0f;
    return {
      state.x - particleSize,
      state.y - particleSize,
      state.x + particleSize,
      state.y + particleSize,
      this->brush.get()
    };
  });

  RenderPoints(renderTarget, explosionParticles.cbegin(), explosionParticles.cend());
}

auto explosion_state::GetOutline() -> object_outline
{
  return outline;
}

[[nodiscard]] auto CreateExplosion(float x, float y, int64_t clockFrequency, screen_render_brush_selector brushes) -> explosion_state
{
  extern std::mt19937 rng;
  const int particleCount = 50;

  explosion_state explosionState { brushes };
  explosionState.clockFrequency = clockFrequency;
  explosionState.particles.resize(particleCount);
  
  for( int i =0; i < particleCount; ++i )
  {
    explosionState.particles[i].condition = particle_state::alive;
    explosionState.particles[i].x = x;
    explosionState.particles[i].y = y;
    float angle = static_cast<float>(particleAngleDist(rng));
    SetParticleVelocity(explosionState.particles[i], angle);
  }

  return explosionState;
}

void SetParticleVelocity(particle_state& particleState, float angle)
{
  extern std::mt19937 rng;
  const auto velocity = static_cast<float>(particleSpeedDist(rng));
  particleState.xVelocity = CalculateVectorX(velocity, angle);
  particleState.yVelocity = CalculateVectorY(velocity, angle);
}

void UpdateState(explosion_state& state, float updateInterval, float forceOfGravity)
{
  for( auto& particle : state.particles )
  {
    if( particle.condition == particle_state::alive )
    {
      particle.yVelocity += forceOfGravity * updateInterval;
      particle.x += particle.xVelocity * updateInterval;
      particle.y += particle.yVelocity * updateInterval;
    }
  }
}

void ProcessCollisions(explosion_state& explosion, const level_ground_geometry& groundGeometry)
{
  for( auto& particle : explosion.particles )
  {
    if( IsUnderground(particle.x, particle.y, groundGeometry) )
      particle.condition = particle_state::dead;
  }
}
