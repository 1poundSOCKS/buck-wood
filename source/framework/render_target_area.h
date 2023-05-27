#pragma once

class render_target_area
{
public:

  enum class align_vertical { centre, top, bottom };
  enum class align_horizontal { centre, left, right };

  render_target_area(D2D1_SIZE_F renderTargetSize, float width, float height, align_vertical alignVertical = align_vertical::centre, align_horizontal alignHorizontal = align_horizontal::centre);
  [[nodiscard]] auto GetRect() const -> D2D1_RECT_F;

private:

  auto GetTopBorder(float totalHeight, float areaHeight, align_vertical alignVertical) -> float;
  auto GetLeftBorder(float totalWidth, float areaWidth, align_horizontal alignHorizontal) -> float;

  D2D1_RECT_F m_rect;
};
