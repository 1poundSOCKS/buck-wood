#ifndef _level_target_
#define _level_target_

#include "framework.h"

struct target_state
{
  target_state(const game_point& position);
  const game_point& position;
  std::vector<game_point> points;
  bool activated = false;
  std::vector<game_line> shape;
};

[[nodiscard]] auto HasCollided(float x, float y, const target_state& target) -> bool;

inline auto HitByBullet(target_state&) -> void
{
}

#endif
