#include "pch.h"
#include "level_target.h"
#include "game_objects.h"

target_state::target_state(const game_point& position) : position(position)
{
  std::vector<game_point> pointsTmp;
  const auto& targetGeometryData = GetDefaultTargetGeometryData();
  TransformPoints(targetGeometryData.cbegin(), targetGeometryData.cend(), std::back_inserter(points), D2D1::Matrix3x2F::Translation(position.x, position.y));
}

[[nodiscard]] auto HasCollided(float x, float y, const target_state& target) -> bool
{
  return PointInside({ x, y }, target.shape.cbegin(), target.shape.cend());
}
