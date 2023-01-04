#ifndef _explosion_
#define _explosion_

#include "geometry.h"
#include "screen_render_data.h"

struct particle_state
{
  float x;
  float y;
  float xVelocity;
  float yVelocity;
};

struct explosion_state
{
  int64_t creationTicks;
  int64_t tickFrequency;
  int64_t updateTicks;
  std::vector<particle_state> particles;
};

[[nodiscard]] auto CreateExplosion(float x, float y, int64_t currentTicks) -> explosion_state [[nothrow]];
void UpdateState(explosion_state& state, int64_t currentTicks) [[nothrow]];
void Render(ID2D1RenderTarget* renderTarget, const screen_render_brushes& renderData, const explosion_state& state) [[nothrow]];

#endif
