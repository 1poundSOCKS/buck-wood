#include "pch.h"
#include "explosion.h"

std::uniform_int_distribution<int> particleAngleDist(0, 359);
std::uniform_int_distribution<int> particleSpeedDist(100, 150);

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
  extern std::mt19937 rng;
  const auto velocity = particleSpeedDist(rng);
  particleState.xVelocity = CalculateVectorX(velocity, angle);
  particleState.yVelocity = CalculateVectorY(velocity, angle);
}

void UpdateState(explosion_state& state, float updateInterval, float forceOfGravity) [[nothrow]]
{
  for( auto& particle : state.particles )
  {
    particle.yVelocity += forceOfGravity * updateInterval;
    particle.x += particle.xVelocity * updateInterval;
    particle.y += particle.yVelocity * updateInterval;
  }
}
