#include "pch.h"
#include "explosion.h"

std::uniform_int_distribution<int> particleAngleDist(0, 359);

void SetParticleVelocity(particle_state& particleState, float angle) [[nothrow]];

[[nodiscard]] auto CreateExplosion(float x, float y, int64_t clockFrequency) -> explosion_state [[nothrow]]
{
  extern std::mt19937 rng;
  const int particleCount = 50;

  explosion_state explosionState;
  explosionState.clockFrequency = clockFrequency;
  explosionState.particles.resize(particleCount);
  
  for( int i =0; i < particleCount; ++i )
  {
    explosionState.particles[i].x = x;
    explosionState.particles[i].y = y;
    float angle = static_cast<float>(particleAngleDist(rng));
    SetParticleVelocity(explosionState.particles[i], angle);
  }

  return explosionState;
}

void SetParticleVelocity(particle_state& particleState, float angle) [[nothrow]]
{
  const auto velocity = 10.0f;
  particleState.xVelocity = CalculateVectorX(velocity, angle);
  particleState.yVelocity = CalculateVectorY(velocity, angle);
}

void UpdateState(explosion_state& state, int64_t elapsedTime) [[nothrow]]
{
  for( auto& particle : state.particles )
  {
    particle.x += particle.xVelocity * static_cast<float>(elapsedTime) / static_cast<float>(state.clockFrequency);
    particle.y += particle.yVelocity * static_cast<float>(elapsedTime) / static_cast<float>(state.clockFrequency);
  }
}
