#include "pch.h"
#include "level_island.h"

level_island::level_island(const game_closed_object& object, screen_render_brush_selector brushes) : object(object)
{
  brush.attach(brushes[brown]);
  brush->AddRef();
}

auto level_island::Update(int64_t tickFrequency, int64_t tickCount, play_event_inserter playEventInserter) -> void
{
}

[[nodiscard]] auto level_island::HasCollided(float x, float y) const -> bool
{
  return PointInside(x, y, object);
}

auto level_island::HitByBullet() -> void
{
}

[[nodiscard]] auto level_island::LevelIsComplete() const -> bool
{
  return true;
}

auto level_island::RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F) const -> void
{
  std::vector<render_line> renderLines;
  CreateRenderLines(object.lines.cbegin(), object.lines.cend(), std::back_inserter(renderLines), brush.get(), 6);
  RenderLines(renderTarget, renderLines.cbegin(), renderLines.cend());
}

[[nodiscard]] auto level_island::GetOutline() -> object_outline
{
  return m_outline;
}

[[nodiscard]] auto level_island::HasCollidedWith(const object_outline& outline) const -> bool
{
  return false;
}

[[nodiscard]] auto level_island::GetCollisionEffect() const -> collision_effect
{
  return {};
}

auto level_island::ApplyCollisionEffect(const collision_effect& effect) -> void
{
}
