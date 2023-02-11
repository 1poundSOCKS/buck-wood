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

[[nodiscard]] auto target_state::HasCollided(float x, float y) const -> bool
{
  return PointInside({ x, y }, shape.cbegin(), shape.cend());
}

auto target_state::HitByBullet() -> void
{
  activated = true;
}

[[nodiscard]] auto target_state::LevelIsComplete() const -> bool
{
  return activated;
}

void target_state::RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F) const
{
  std::vector<render_line> renderLines;
  CreateRenderLines(shape.cbegin(), shape.cend(), std::back_inserter(renderLines), activated ? brushActivated.get() : brushNotActivated.get(), 6);
  RenderLines(renderTarget, renderLines.cbegin(), renderLines.cend());
}
