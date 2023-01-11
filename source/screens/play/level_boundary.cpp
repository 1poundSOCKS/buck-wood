#include "pch.h"
#include "level_boundary.h"
#include "collisions.h"

[[nodiscard]] auto HasCollided(float x, float y, const level_boundary& levelBoundary) -> bool
{
  return !HasCollided(x, y, levelBoundary.object);
}
