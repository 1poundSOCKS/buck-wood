#include "pch.h"
#include "render_target_area.h"

render_target_area::render_target_area(D2D1_SIZE_F renderTargetSize, float width, float height, align_vertical alignVertical, align_horizontal alignHorizontal)
{
  auto realWidth = renderTargetSize.width * width;
  auto realHeight = renderTargetSize.height * height;

  auto topBorder = GetTopBorder(renderTargetSize.height, realHeight, alignVertical);
  auto leftBorder = GetLeftBorder(renderTargetSize.width, realWidth, alignHorizontal);

  m_rect = { leftBorder, topBorder, leftBorder + realWidth, topBorder + realHeight };
}

[[nodiscard]] auto render_target_area::GetRect() const -> D2D1_RECT_F
{
  return m_rect;
}

auto render_target_area::GetTopBorder(float totalHeight, float areaHeight, align_vertical alignVertical) -> float
{
  switch( alignVertical )
  {
    case vertical_centre:
      return ( totalHeight - areaHeight ) / 2.0f;
    case vertical_top:
      return 0.0f;
    case vertical_bottom:
      return totalHeight - areaHeight;
    default:
      return 0;
  }
}

auto render_target_area::GetLeftBorder(float totalWidth, float areaWidth, align_horizontal alignHorizontal) -> float
{
  switch( alignHorizontal )
  {
    case horizontal_centre:
      return ( totalWidth - areaWidth ) / 2.0f;
    case horizontal_left:
      return 0.0f;
    case horizontal_right:
      return totalWidth - areaWidth;
    default:
      return 0;
  }
}
