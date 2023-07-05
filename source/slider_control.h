#pragma once

class slider_control
{

public:

  enum class alignment { horizontal, vertical };

  slider_control(const D2D1_RECT_F& rect );

  [[nodiscard]] auto GetBoundingRect() const -> const D2D1_RECT_F&;
  [[nodiscard]] auto GetSliderRect(float percentage) const -> D2D1_RECT_F;

private:

  [[nodiscard]] auto GetSliderTop(float percentage) const -> float;
  [[nodiscard]] auto GetSliderRight(float percentage) const -> float;

  alignment m_alignment { alignment::vertical };
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
  switch( m_alignment )
  {
  case alignment::vertical:
    return { m_rect.left, GetSliderTop(percentage), m_rect.right, m_rect.bottom };
    
  case alignment::horizontal:
    return { m_rect.left, m_rect.top, GetSliderRight(percentage), m_rect.bottom };

  default:
    return m_rect;
  }
}

[[nodiscard]] inline auto slider_control::GetSliderTop(float percentage) const -> float
{
  return m_rect.bottom - ( m_rect.bottom - m_rect.top ) * percentage / 100.0f;
}

[[nodiscard]] inline auto slider_control::GetSliderRight(float percentage) const -> float
{
  return m_rect.left + ( m_rect.right - m_rect.left ) * percentage / 100.0f;
}
