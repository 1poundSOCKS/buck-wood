#include "pch.h"
#include "level_background.h"
#include "render_brush_defs.h"

constexpr D2D1_RECT_F GetStarRect()
{
  return { -6, -6, 6, 6 };
}

auto level_background::AddStar(game_point position) -> void
{
  m_stars.emplace_back(position);
}

auto level_background::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_starBrush =  screen_render_brush_dark_grey.CreateBrush(renderTarget);
}

auto level_background::Update(const object_input_data& inputData, int64_t tickCount) -> void
{
}

auto level_background::Render(D2D1_RECT_F viewRect) const -> void
{
  const D2D1_RECT_F rect = GetStarRect();

  for( const auto& star : m_stars )
  {
    m_renderTarget->FillRectangle(D2D1_RECT_F { rect.left + star.x, rect.top + star.y, rect.right + star.x, rect.bottom + star.y }, m_starBrush.get());
  }
}
