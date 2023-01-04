#include "pch.h"
#include "explosion.h"
#include "framework.h"

std::uniform_int_distribution<int> particleAngleDist(0, 359);

void SetParticleVelocity(particle_state& particleState, float angle) [[nothrow]];

[[nodiscard]] auto CreateExplosion(float x, float y, int64_t currentTicks, int64_t tickFrequency) -> explosion_state [[nothrow]]
{
  extern std::mt19937 rng;
  float angle = static_cast<float>(particleAngleDist(rng));

  const int particleCount = 10;

  explosion_state explosionState;
  explosionState.creationTicks = currentTicks;
  explosionState.tickFrequency = tickFrequency;
  explosionState.particles.resize(particleCount);
  
  for( int i =0; i < particleCount; ++i )
  {
    explosionState.particles[i].x = x;
    explosionState.particles[i].y = y;
    SetParticleVelocity(explosionState.particles[i], angle);
  }

  return explosionState;
}

void SetParticleVelocity(particle_state& particleState, float angle) [[nothrow]]
{
  const auto velocity = 1.0f;
  particleState.xVelocity = CalculateVectorX(velocity, angle);
  particleState.yVelocity = CalculateVectorY(velocity, angle);
}

void UpdateState(explosion_state& state, int64_t currentTicks) [[nothrow]]
{
  auto updateInterval = state.updateTicks - currentTicks;
  
  for( auto& particle : state.particles )
  {
    particle.x += particle.xVelocity * static_cast<float>(updateInterval) / static_cast<float>(state.tickFrequency);
    particle.y += particle.yVelocity * static_cast<float>(updateInterval) / static_cast<float>(state.tickFrequency);
  }

  state.updateTicks = currentTicks;
}

void Render(ID2D1RenderTarget* renderTarget, const screen_render_brushes& brushes, const explosion_state& state) [[nothrow]]
{
  screen_render_brush_selector brushSelector(brushes);
  auto brush = brushSelector[white];

  std::vector<render_rect> renderParticles;
  std::transform(state.particles.cbegin(), state.particles.cend(), std::back_inserter(renderParticles), 
  [renderTarget, brush](const particle_state& state) -> render_rect
  {
    const auto particleSize = 10.0f;
    return {
      state.x - particleSize,
      state.y - particleSize,
      state.x + particleSize,
      state.y + particleSize,
      brush
    };
  });

  RenderPoints(renderTarget, renderParticles.cbegin(), renderParticles.cend());
}
