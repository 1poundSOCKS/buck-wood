#include "pch.h"
#include "level_island.h"
#include "render_brush_defs.h"

level_island::level_island(const game_closed_object& object) : m_object(object), m_collisionData(object)
{
  m_collisionEffect.SetProperty(collision_effect::stops_bullets, true);
  m_collisionEffect.SetProperty(collision_effect::kills_player, true);

  m_geometry = framework::CreatePathGeometry();

  winrt::com_ptr<ID2D1GeometrySink> sink;
  m_geometry->Open(sink.put());

  sink->BeginFigure({ object.points.front().x, object.points.front().y }, D2D1_FIGURE_BEGIN_FILLED);

  for( auto point = std::next(object.points.cbegin()); point != object.points.cend(); ++point )
  {
    sink->AddLine({ point->x, point->y });
  }

  sink->EndFigure(D2D1_FIGURE_END_CLOSED);

  sink->Close();
}

auto level_island::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_fillBrush = screen_render_brush_grey.CreateBrush(renderTarget);
  m_borderBrush = screen_render_brush_dark_grey.CreateBrush(renderTarget);
}

auto level_island::Update(const object_input_data& inputData, int64_t tickCount) -> void
{
}

auto level_island::Render(D2D1_RECT_F) const -> void
{
  m_renderTarget->FillGeometry(m_geometry.get(), m_fillBrush.get());
  m_renderTarget->DrawGeometry(m_geometry.get(), m_borderBrush.get(), 6.0f);
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
