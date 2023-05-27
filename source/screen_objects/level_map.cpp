#include "pch.h"
#include "level_map.h"
#include "framework.h"
#include "render_brush_defs.h"

level_map::level_map() : m_data { std::make_shared<data>() }
{
  const auto& renderTarget = framework::renderTarget();
  m_brush = screen_render_brush_white.CreateBrush(renderTarget.get());
}

auto level_map::SetRect(game_rect rect) -> void
{
  m_data->rect = rect;
}

auto level_map::SetPlayer(float x, float y) -> void
{
  m_data->playerX = x;
  m_data->playerY = y;
}

auto level_map::Update(const object_input_data& inputData, int64_t clockCount) -> void
{

}

auto level_map::Render(D2D1_RECT_F viewRect) const -> void
{
  const auto& renderTarget = framework::renderTarget();
  auto rect = D2D1_RECT_F { m_data->rect.topLeft.x, m_data->rect.topLeft.y, m_data->rect.bottomRight.x, m_data->rect.bottomRight.y };
  renderTarget->DrawRectangle( rect, m_brush.get() );
}
