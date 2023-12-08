#pragma once

class render_target_area
{
public:

  class constraint_centred
  {
  public:

    constraint_centred(float width, float height);

    [[nodiscard]] auto GetLeftBorder(float width) const -> float;
    [[nodiscard]] auto GetTopBorder(float height) const -> float;
    [[nodiscard]] auto GetRightBorder(float width) const -> float;
    [[nodiscard]] auto GetBottomBorder(float height) const -> float;

  private:
    float m_width { 0 };
    float m_height { 0 };
  };

  class constraint_bottom_centre
  {
  public:

    constraint_bottom_centre(float width, float height);

    [[nodiscard]] auto GetLeftBorder(float width) const -> float;
    [[nodiscard]] auto GetTopBorder(float height) const -> float;
    [[nodiscard]] auto GetRightBorder(float width) const -> float;
    [[nodiscard]] auto GetBottomBorder(float height) const -> float;

  private:
    float m_width { 0 };
    float m_height { 0 };
  };

  class contraint_bottom_right
  {
  public:

    contraint_bottom_right(float width, float height);

    [[nodiscard]] auto GetLeftBorder(float width) const -> float;
    [[nodiscard]] auto GetTopBorder(float height) const -> float;
    [[nodiscard]] auto GetRightBorder(float width) const -> float;
    [[nodiscard]] auto GetBottomBorder(float height) const -> float;

  private:
    float m_width { 0 };
    float m_height { 0 };
  };

  enum class align_vertical { centre, top, bottom };
  enum class align_horizontal { centre, left, right };

  render_target_area(D2D1_SIZE_F renderTargetSize);
  template <typename constraint_type> render_target_area(D2D1_SIZE_F renderTargetSize, const constraint_type& constraint);
  template <typename constraint_type> render_target_area(D2D1_RECT_F renderTargetRect, const constraint_type& constraint);

  [[nodiscard]] auto GetRect() const -> D2D1_RECT_F;

private:

  template <typename constraint_type> auto Initialize(D2D1_RECT_F rect, const constraint_type& constraint) -> void;

  auto GetTopBorder(float totalHeight, float areaHeight, align_vertical alignVertical) -> float;
  auto GetLeftBorder(float totalWidth, float areaWidth, align_horizontal alignHorizontal) -> float;

  D2D1_RECT_F m_rect;
};

template <typename constraint_type> render_target_area::render_target_area(D2D1_SIZE_F renderTargetSize, const constraint_type& constraint) : m_rect { 0, 0, renderTargetSize.width - 1, renderTargetSize.height - 1 }
{
  Initialize(m_rect, constraint);
}

template <typename constraint_type> render_target_area::render_target_area(D2D1_RECT_F rect, const constraint_type& constraint) : m_rect(rect)
{
  Initialize(m_rect, constraint);
}

template <typename constraint_type> auto render_target_area::Initialize(D2D1_RECT_F rect, const constraint_type& constraint) -> void
{
  auto width = rect.right - rect.left + 1;
  auto height = rect.bottom - rect.top + 1;

  auto leftBorder = constraint.GetLeftBorder(width);
  auto rightBorder = constraint.GetRightBorder(width);
  auto topBorder = constraint.GetTopBorder(height);
  auto bottomBorder = constraint.GetBottomBorder(height);

  m_rect = { rect.left + leftBorder, rect.top + topBorder, rect.right - rightBorder, rect.bottom - bottomBorder };
}
