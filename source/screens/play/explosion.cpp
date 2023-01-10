#include "pch.h"
#include "explosion.h"

std::uniform_int_distribution<int> particleAngleDist(0, 359);
std::uniform_int_distribution<int> particleSpeedDist(150, 200);

void SetParticleVelocity(particle_state& particleState, float angle);

[[nodiscard]] auto CreateExplosion(float x, float y, int64_t clockFrequency) -> explosion_state
{
  extern std::mt19937 rng;
  const int particleCount = 50;

  explosion_state explosionState;
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
