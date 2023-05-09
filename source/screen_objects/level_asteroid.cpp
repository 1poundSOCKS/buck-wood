#include "pch.h"
#include "level_asteroid.h"
#include "render_brush_defs.h"

level_asteroid::level_asteroid(const game_closed_object& object) : m_object(object), m_collisionData(object), m_geometry(object)
{
  m_collisionEffect.SetProperty(collision_effect::stops_bullets, true);
  m_collisionEffect.SetProperty(collision_effect::kills_player, true);
}

auto level_asteroid::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_fillBrush = screen_render_brush_grey.CreateBrush(renderTarget);
  m_borderBrush = screen_render_brush_dark_grey.CreateBrush(renderTarget);
}

auto level_asteroid::Update(const object_input_data& inputData, int64_t tickCount) -> void
{
}

auto level_asteroid::Render(D2D1_RECT_F) const -> void
{
  m_renderTarget->FillGeometry(m_geometry.Get(), m_fillBrush.get());
  m_renderTarget->DrawGeometry(m_geometry.Get(), m_borderBrush.get(), 6.0f);
}

[[nodiscard]] auto level_asteroid::GetCollisionData() const -> const collision_data&
{
  return m_collisionData;
}

[[nodiscard]] auto level_asteroid::HasCollidedWith(const collision_data& collisionData) const -> bool
{
  return false;
}

[[nodiscard]] auto level_asteroid::GetCollisionEffect() const -> collision_effect
{
  return m_collisionEffect;
}

auto level_asteroid::ApplyCollisionEffect(const collision_effect& effect) -> void
{
}

[[nodiscard]] auto level_asteroid::Destroyed() const -> bool
{
  return false;
}
