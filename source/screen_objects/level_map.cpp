#include "pch.h"
#include "level_map.h"
#include "framework.h"
#include "render_brush_defs.h"

level_map::level_map() : m_data { std::make_shared<data>() }
{
  const auto& renderTarget = framework::renderTarget();
  m_brush = screen_render_brush_white.CreateBrush(renderTarget.get());
  m_targetBrush = screen_render_brush_green.CreateBrush(renderTarget.get());
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
  auto renderTargetSize = renderTarget->GetSize();
  auto rect = D2D1_RECT_F { m_data->rect.topLeft.x, m_data->rect.topLeft.y, m_data->rect.bottomRight.x, m_data->rect.bottomRight.y };
  renderTarget->DrawRectangle( rect, m_brush.get(), 3.0f );

  auto mapCentre = m_data->rect.CentrePoint();
  auto playerRect = D2D1_RECT_F { GetPlayerRect().left + mapCentre.x,  GetPlayerRect().top + mapCentre.y, GetPlayerRect().right + mapCentre.x, GetPlayerRect().bottom + mapCentre.y };
  renderTarget->FillRectangle(playerRect, m_brush.get());

  auto transform = D2D1::Matrix3x2F::Translation(-m_data->playerX, -m_data->playerY) * D2D1::Matrix3x2F::Scale(0.02f, 0.02f) * D2D1::Matrix3x2F::Translation(mapCentre.x, mapCentre.y);

  for( const auto& target : m_data->targets )
  {
    auto point = transform.TransformPoint({target.x, target.y});
    auto targetRect = D2D1_RECT_F { GetTargetRect().left + point.x,  GetTargetRect().top + point.y, GetTargetRect().right + point.x, GetTargetRect().bottom + point.y };
    renderTarget->FillRectangle(targetRect, m_targetBrush.get());
  }
}

constexpr auto level_map::GetPlayerRect() -> D2D1_RECT_F
{
  return { -2, -2, 2, 2 };
}

constexpr auto level_map::GetTargetRect() -> D2D1_RECT_F
{
  return { -3, -3, 3, 3 };
}
