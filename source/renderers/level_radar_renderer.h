#pragma once

#include "level_radar.h"
#include "color_scale_brushes.h"

class level_radar_renderer
{

public:

  level_radar_renderer() : m_brushes { color_scale { D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f), D2D1::ColorF(0.0f, 0.5f, 0.0f, 1.0f), 10 } }
  {
  }

  auto Write(const level_radar& levelRadar, std::ranges::input_range auto&& targets) const -> void;

private:

  color_scale_brushes m_brushes;

};

auto level_radar_renderer::Write(const level_radar& levelRadar, std::ranges::input_range auto&& targets) const -> void
{
  struct radar_target
  {
    float angle { 0 };
    float distance { 0 };
  };

  auto renderTargetSize = render_target::get()->GetSize();
  auto centreX = renderTargetSize.width / 2;
  auto centreY = renderTargetSize.height / 2;

  auto targetsNotActivated = targets | std::ranges::views::filter([](const auto& target)
  {
    return !target.IsActivated();
  });

  auto radarTargets = targetsNotActivated | std::ranges::views::transform([&levelRadar](const auto& target) -> radar_target
  {
    return { levelRadar.AngleTo(target.Position()), levelRadar.DistanceTo(target.Position()) };
  });

  for( auto radarTarget : radarTargets )
  {
    D2D1_POINT_2F targetPoint { 0, -250 };
    D2D1::Matrix3x2F transform = D2D1::Matrix3x2F::Rotation(radarTarget.angle) * D2D1::Matrix3x2F::Translation(centreX, centreY);
    auto renderPoint = transform.TransformPoint(targetPoint);
    D2D1_RECT_F renderRect = { renderPoint.x - 5, renderPoint.y - 5, renderPoint.x + 5, renderPoint.y + 5 };
    auto distanceRatio = min(1.0f, radarTarget.distance / 5000.0f);
    render_target::get()->FillRectangle(renderRect, m_brushes[distanceRatio].get());
  }
}
