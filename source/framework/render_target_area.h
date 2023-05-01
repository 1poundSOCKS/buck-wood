#pragma once

class render_target_area
{
public:

  enum align_vertical { vertical_centre, vertical_top, vertical_bottom };
  enum align_horizontal { horizontal_centre, horizontal_left, horizontal_right };

  render_target_area(D2D1_SIZE_F renderTargetSize, float width, float height, align_vertical alignVertical = vertical_centre, align_horizontal alignHorizontal = horizontal_centre);
  [[nodiscard]] auto GetRect() const -> D2D1_RECT_F;

private:

  auto GetTopBorder(float totalHeight, float areaHeight, align_vertical alignVertical) -> float;
  auto GetLeftBorder(float totalWidth, float areaWidth, align_horizontal alignHorizontal) -> float;

  D2D1_RECT_F m_rect;
};
