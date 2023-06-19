#pragma once

#include "framework.h"
#include "geometry.h"

class level_radar
{
public:

  level_radar();
  auto Render(game_point playerPosition, std::ranges::input_range auto&& targets) const -> void;

private:

  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  
};

auto level_radar::Render(game_point playerPosition, std::ranges::input_range auto&& targets) const -> void
{
  const auto& renderTarget = framework::renderTarget();
  auto renderTargetSize = renderTarget->GetSize();
  auto centreX = renderTargetSize.width / 2;
  auto centreY = renderTargetSize.height / 2;

  auto targetAngleView = targets | 
  std::ranges::views::filter([](const auto& target)
  {
    return !target.IsActivated();
  }) | 
  std::ranges::views::transform([&playerPosition](const auto& target) -> float
  {
    return playerPosition.AngleTo(target.Position());
  });

  for( auto targetAngle : targetAngleView )
  {
    D2D1_POINT_2F targetPoint { 0, -250 };
    D2D1::Matrix3x2F transform = D2D1::Matrix3x2F::Rotation(targetAngle) * D2D1::Matrix3x2F::Translation(centreX, centreY);
    auto renderPoint = transform.TransformPoint(targetPoint);
    D2D1_RECT_F renderRect = { renderPoint.x - 5, renderPoint.y - 5, renderPoint.x + 5, renderPoint.y + 5 };
    renderTarget->FillRectangle(renderRect, m_brush.get());
  }
}
