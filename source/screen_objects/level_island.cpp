#include "pch.h"
#include "level_island.h"
#include "render_brush_defs.h"

level_island::level_island(const game_closed_object& object) : object(object), m_collisionData(object)
{
  m_collisionEffect.SetProperty(collision_effect::stops_bullets, true);
  m_collisionEffect.SetProperty(collision_effect::kills_player, true);
}

auto level_island::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  brush = screen_render_brush_brown.CreateBrush(renderTarget);
}

auto level_island::Update(const object_input_data& inputData, int64_t tickCount) -> void
{
}

auto level_island::Render(D2D1_RECT_F) const -> void
{
  std::vector<render_line> renderLines;
  CreateRenderLines(object.lines.cbegin(), object.lines.cend(), std::back_inserter(renderLines), brush.get(), 6);
  RenderLines(m_renderTarget.get(), renderLines.cbegin(), renderLines.cend());
}

[[nodiscard]] auto level_island::GetCollisionData() -> collision_data
{
  return m_collisionData;
}

[[nodiscard]] auto level_island::HasCollidedWith(const collision_data& collisionData) const -> bool
{
  return false;
}

[[nodiscard]] auto level_island::GetCollisionEffect() const -> collision_effect
{
  return m_collisionEffect;
}

auto level_island::ApplyCollisionEffect(const collision_effect& effect) -> void
{
}

[[nodiscard]] auto level_island::Destroyed() const -> bool
{
  return false;
}
