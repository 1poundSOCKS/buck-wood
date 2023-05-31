#pragma once

#include "object_input_data.h"
#include "geometry.h"
#include "framework.h"
#include "render_brush_defs.h"

template <typename item_type>
class level_map
{

public:

  level_map();

  auto SetRect(game_rect rect) -> void;

  auto Update(game_point centrePoint, std::ranges::input_range auto&& items) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  static constexpr auto GetPlayerRect() -> D2D1_RECT_F;
  static constexpr auto GetTargetRect() -> D2D1_RECT_F;

  game_rect m_rect { { 0, 0 }, { 0, 0 } };
  float m_playerX { 0 };
  float m_playerY { 0 };
  std::vector<item_type> m_items;

  winrt::com_ptr<ID2D1SolidColorBrush> m_borderBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_playerBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_activatedTargetBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_deactivatedTargetBrush;

};

template <typename item_type>
level_map<item_type>::level_map()
{
  const auto& renderTarget = framework::renderTarget();
  m_borderBrush = screen_render_brush_white.CreateBrush(renderTarget.get());
  m_playerBrush = screen_render_brush_yellow.CreateBrush(renderTarget.get());
  m_activatedTargetBrush = screen_render_brush_red.CreateBrush(renderTarget.get());
  m_deactivatedTargetBrush = screen_render_brush_green.CreateBrush(renderTarget.get());
}

template <typename item_type>
auto level_map<item_type>::SetRect(game_rect rect) -> void
{
  m_rect = rect;
}

template <typename item_type>
auto level_map<item_type>::Update(game_point centrePoint, std::ranges::input_range auto&& items) -> void
{
  m_playerX = centrePoint.x;
  m_playerY = centrePoint.y;

  m_items.clear();
  std::ranges::copy(items, std::back_inserter(m_items));
}

template <typename item_type>
auto level_map<item_type>::Render(D2D1_RECT_F viewRect) const -> void
{
  const auto& renderTarget = framework::renderTarget();
  auto renderTargetSize = renderTarget->GetSize();

  auto rect = D2D1_RECT_F { m_rect.topLeft.x, m_rect.topLeft.y, m_rect.bottomRight.x, m_rect.bottomRight.y };
  renderTarget->DrawRectangle( rect, m_borderBrush.get(), 3.0f );

  auto mapCentre = m_rect.CentrePoint();
  auto playerRect = D2D1_RECT_F { GetPlayerRect().left + mapCentre.x,  GetPlayerRect().top + mapCentre.y, GetPlayerRect().right + mapCentre.x, GetPlayerRect().bottom + mapCentre.y };
  renderTarget->FillRectangle(playerRect, m_playerBrush.get());

  auto transform = D2D1::Matrix3x2F::Translation(-m_playerX, -m_playerY) * D2D1::Matrix3x2F::Scale(0.02f, 0.02f) * D2D1::Matrix3x2F::Translation(mapCentre.x, mapCentre.y);

  for( const auto& item : m_items )
  {
    auto position = item.Position();
    auto point = transform.TransformPoint({position.x, position.y});
    auto targetRect = D2D1_RECT_F { GetTargetRect().left + point.x,  GetTargetRect().top + point.y, GetTargetRect().right + point.x, GetTargetRect().bottom + point.y };
    const auto& brush = item.IsActivated() ? m_activatedTargetBrush : m_deactivatedTargetBrush;
    renderTarget->FillRectangle(targetRect, brush.get());
  }
}

template <typename item_type>
constexpr auto level_map<item_type>::GetPlayerRect() -> D2D1_RECT_F
{
  return { -2, -2, 2, 2 };
}

template <typename item_type>
constexpr auto level_map<item_type>::GetTargetRect() -> D2D1_RECT_F
{
  return { -3, -3, 3, 3 };
}
