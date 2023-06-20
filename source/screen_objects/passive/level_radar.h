#pragma once

#include "framework.h"
#include "geometry.h"
#include "color_scale_brushes.h"

class level_radar
{
public:

  level_radar();
  auto Render(game_point playerPosition, std::ranges::input_range auto&& targets) const -> void;

private:

  color_scale_brushes m_brushes;
  
};

auto level_radar::Render(game_point playerPosition, std::ranges::input_range auto&& targets) const -> void
{
  struct radar_target
  {
    float angle { 0 };
    float distance { 0 };
  };

  const auto& renderTarget = framework::renderTarget();
  auto renderTargetSize = renderTarget->GetSize();
  auto centreX = renderTargetSize.width / 2;
  auto centreY = renderTargetSize.height / 2;

  auto radarTargetsView = targets | 
  std::ranges::views::filter([](const auto& target)
  {
    return !target.IsActivated();
  }) | 
  std::ranges::views::transform([&playerPosition](const auto& target) -> radar_target
  {
    return { playerPosition.AngleTo(target.Position()), playerPosition.DistanceTo(target.Position()) };
  });

  for( auto radarTarget : radarTargetsView )
  {
    D2D1_POINT_2F targetPoint { 0, -250 };
    D2D1::Matrix3x2F transform = D2D1::Matrix3x2F::Rotation(radarTarget.angle) * D2D1::Matrix3x2F::Translation(centreX, centreY);
    auto renderPoint = transform.TransformPoint(targetPoint);
    D2D1_RECT_F renderRect = { renderPoint.x - 5, renderPoint.y - 5, renderPoint.x + 5, renderPoint.y + 5 };
    auto distanceRatio = min(1.0f, radarTarget.distance / 5000.0f);

    renderTarget->FillRectangle(renderRect, m_brushes[distanceRatio].get());
  }
}
