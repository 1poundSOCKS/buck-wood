#include "pch.h"
#include "level_objects.h"
#include "game_objects.h"

target_state::target_state(const game_point& position) : position(position)
{
  std::vector<game_point> pointsTmp;
  CreatePointsForTarget(defaultTargetSize, std::back_inserter(pointsTmp));
  TransformPoints(pointsTmp.cbegin(), pointsTmp.cend(), std::back_inserter(points), 0, position.x, position.y);
}
