#pragma once

class slider_control
{

public:

  slider_control(const D2D1_RECT_F& rect);

  [[nodiscard]] auto GetBoundingRect() const -> const D2D1_RECT_F&;
  [[nodiscard]] auto GetSliderRect(float percentage) const -> D2D1_RECT_F;

private:

  D2D1_RECT_F m_rect { 0, 0, 0, 0 };

};

inline slider_control::slider_control(const D2D1_RECT_F& rect) : m_rect { rect }
{
}

[[nodiscard]] auto slider_control::GetBoundingRect() const -> const D2D1_RECT_F&
{
  return m_rect;
}

[[nodiscard]] auto slider_control::GetSliderRect(float percentage) const -> D2D1_RECT_F
{
  auto right = ( m_rect.right - m_rect.left ) * percentage / 100.0f + m_rect.left;
  return { m_rect.left, m_rect.top, right, m_rect.bottom };
}
