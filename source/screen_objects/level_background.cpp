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
}

auto level_background::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_starBrush =  screen_render_brush_white.CreateBrush(renderTarget);
}

auto level_background::Update(const object_input_data& inputData, int64_t tickCount) -> void
{
  m_stars.clear();

  auto left = static_cast<int>(m_x) - 500;
  auto right = static_cast<int>(m_x) + 500;
  auto top = static_cast<int>(m_y) - 500;
  auto bottom = static_cast<int>(m_y) + 500;

  game_level_object_generator backgroundGenerator(left / 20, right / 20, 20, top / 20, bottom / 20, 20, 0.98f, 1.0f, 3.0f);

  backgroundGenerator.InsertInto(std::back_inserter(m_stars));
}

auto level_background::Render(D2D1_RECT_F viewRect) const -> void
{
  const D2D1_RECT_F rect = GetStarRect();

  for( const auto& star : m_stars )
  {
    m_renderTarget->FillRectangle(D2D1_RECT_F { rect.left + star.x, rect.top + star.y, rect.right + star.x, rect.bottom + star.y }, m_starBrush.get());
  }
}

auto level_background::SetCentre(float x, float y) -> void
{
  m_x = x;
  m_y = y;
}
