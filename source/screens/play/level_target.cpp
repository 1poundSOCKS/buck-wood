#include "pch.h"
#include "level_target.h"
#include "game_objects.h"

target_state::target_state(const game_point& position, screen_render_brush_selector brushes) : position(position)
{
  std::vector<game_point> pointsTmp;
  const auto& targetGeometryData = GetDefaultTargetGeometryData();
  TransformPoints(targetGeometryData.cbegin(), targetGeometryData.cend(), std::back_inserter(points), D2D1::Matrix3x2F::Translation(position.x, position.y));

  brushNotActivated.attach(brushes[green]);
  brushNotActivated->AddRef();
  brushActivated.attach(brushes[red]);
  brushActivated->AddRef();
}

[[nodiscard]] auto HasCollided(float x, float y, const target_state& target) -> bool
{
  return PointInside({ x, y }, target.shape.cbegin(), target.shape.cend());
}

auto HitByBullet(target_state& targetState) -> void
{
  targetState.activated = true;
}

auto GetRenderLines(const target_state& targetState, render_line_inserter_type inserter) -> void
{
  auto renderBrush = targetState.activated ? targetState.brushActivated : targetState.brushNotActivated;
  CreateRenderLines(targetState.shape.cbegin(), targetState.shape.cend(), inserter, renderBrush.get(), 4);
}
