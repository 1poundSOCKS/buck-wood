#ifndef _explosion_
#define _explosion_

#include "geometry.h"
#include "screen_render_data.h"
#include "render.h"
#include "level_geometry.h"

struct particle_state
{
  enum condition_type { alive, dead };
  condition_type condition;
  float x;
  float y;
  float xVelocity;
  float yVelocity;
};

struct explosion_state
{
  int64_t clockFrequency;
  std::vector<particle_state> particles;
};

[[nodiscard]] auto CreateExplosion(float x, float y, int64_t clockFrequency) -> explosion_state [[nothrow]];
void UpdateState(explosion_state& state, float updateInterval, float forceOfGravity) [[nothrow]];

void CreateRenderPoints(const explosion_state& state, const screen_render_brushes& brushes, auto renderPointsInserter) [[nothrow]]
{
  std::vector<particle_state> particles;
  std::copy_if(state.particles.cbegin(), state.particles.cend(), std::back_inserter(particles), 
  [](const particle_state& particle) -> bool
  {
    return particle.condition == particle_state::alive;
  });

  screen_render_brush_selector brushSelector(brushes);
  auto brush = brushSelector[white];

  std::transform(particles.cbegin(), particles.cend(), renderPointsInserter, 
  [brush](const particle_state& state) -> render_rect
  {
    const auto particleSize = 6.0f;
    return {
      state.x - particleSize,
      state.y - particleSize,
      state.x + particleSize,
      state.y + particleSize,
      brush
    };
  });
}

void CreateRenderPoints(auto explosionBegin, auto explosionEnd, const screen_render_brushes& brushes, auto renderPointsInserter) [[nothrow]]
{
  for( auto explosion = explosionBegin; explosion != explosionEnd; ++explosion )
  {
    CreateRenderPoints(*explosion, brushes, renderPointsInserter);
  }
}

void ProcessCollisions(explosion_state& explosion, const level_ground_geometry& groundGeometry);

#endif
