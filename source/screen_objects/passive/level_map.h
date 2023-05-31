#pragma once

#include "object_input_data.h"
#include "geometry.h"
#include "framework.h"
#include "render_brush_defs.h"

class level_map
{

public:

  level_map();
  auto SetRect(game_rect rect) -> void;
  auto Render(game_point playerPosition, std::ranges::input_range auto&& items, D2D1_RECT_F viewRect) const -> void;

private:

  static constexpr auto GetPlayerRect() -> D2D1_RECT_F;
  static constexpr auto GetTargetRect() -> D2D1_RECT_F;

  game_rect m_rect { { 0, 0 }, { 0, 0 } };

  winrt::com_ptr<ID2D1SolidColorBrush> m_borderBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_playerBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_activatedTargetBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_deactivatedTargetBrush;

};

auto level_map::Render(game_point playerPosition, std::ranges::input_range auto&& items, D2D1_RECT_F viewRect) const -> void
{
  const auto& renderTarget = framework::renderTarget();
  auto renderTargetSize = renderTarget->GetSize();

  auto rect = D2D1_RECT_F { m_rect.topLeft.x, m_rect.topLeft.y, m_rect.bottomRight.x, m_rect.bottomRight.y };
  renderTarget->DrawRectangle( rect, m_borderBrush.get(), 3.0f );

  auto mapCentre = m_rect.CentrePoint();
  const auto playerRect = GetPlayerRect();
  auto absPlayerRect = D2D1_RECT_F { playerRect.left + mapCentre.x,  playerRect.top + mapCentre.y, playerRect.right + mapCentre.x, playerRect.bottom + mapCentre.y };
  renderTarget->FillRectangle(absPlayerRect, m_playerBrush.get());

  auto transform = D2D1::Matrix3x2F::Translation(-playerPosition.x, -playerPosition.y) * D2D1::Matrix3x2F::Scale(0.02f, 0.02f) * D2D1::Matrix3x2F::Translation(mapCentre.x, mapCentre.y);

  for( const auto& item : items )
  {
    auto position = item.Position();
    auto point = transform.TransformPoint({position.x, position.y});
    const auto targetRect = GetTargetRect();
    auto absTargetRect = D2D1_RECT_F { targetRect.left + point.x,  targetRect.top + point.y, targetRect.right + point.x, targetRect.bottom + point.y };
    const auto& brush = item.IsActivated() ? m_activatedTargetBrush : m_deactivatedTargetBrush;
    renderTarget->FillRectangle(absTargetRect, brush.get());
  }
}

constexpr auto level_map::GetPlayerRect() -> D2D1_RECT_F
{
  return { -2, -2, 2, 2 };
}

constexpr auto level_map::GetTargetRect() -> D2D1_RECT_F
{
  return { -2, -2, 2, 2 };
}
