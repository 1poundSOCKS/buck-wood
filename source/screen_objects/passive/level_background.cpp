#include "pch.h"
#include "level_background.h"
#include "render_brush_defs.h"
#include "game_level_object_generator.h"

constexpr D2D1_RECT_F GetStarRect()
{
  return { -4, -4, 4, 4 };
}

level_background::level_background()
{
  Initialize(framework::renderTarget().get());
}

auto level_background::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_starBrush =  screen_render_brush_white.CreateBrush(renderTarget);
}

auto level_background::Update(int64_t tickCount) -> void
{
  m_stars.clear();

  const int viewLeft = 1000 / 2;
  const int viewRight = viewLeft;
  const int viewTop = 1000 / 2;
  const int viewBottom = viewTop;

  const int columnWidth = 20;
  const int rowHeight = 20;

  auto leftColumn = ( static_cast<int>(m_x) - viewLeft ) / columnWidth;
  auto rightColumn = ( static_cast<int>(m_x) + viewRight ) / columnWidth;
  auto topRow = ( static_cast<int>(m_y) - viewTop ) / rowHeight;
  auto bottomRow = ( static_cast<int>(m_y) + viewBottom ) / rowHeight;

  game_level_object_generator backgroundGenerator(leftColumn, rightColumn, columnWidth, topRow, bottomRow, rowHeight, 0.98f, 1.0f, 3.0f);

  backgroundGenerator.InsertInto(std::back_inserter(m_stars));
}

auto level_background::Render(D2D1_RECT_F viewRect) const -> void
{
  const D2D1_RECT_F rect = GetStarRect();

  for( const auto& star : m_stars )
  {
    framework::renderTarget()->FillRectangle(D2D1_RECT_F { rect.left + star.x, rect.top + star.y, rect.right + star.x, rect.bottom + star.y }, m_starBrush.get());
  }
}

auto level_background::SetCentre(float x, float y) -> void
{
  m_x = x;
  m_y = y;
}
