#pragma once

class setting_slider
{

public:

  setting_slider() = default;
  setting_slider(int min, int max, int value);
  setting_slider(int min, int max, int value, const D2D1_RECT_F& rect);

  [[nodiscard]] auto Min() const -> int;
  [[nodiscard]] auto Max() const -> int;
  [[nodiscard]] auto Value() const -> int;

  auto Increment() -> int;
  auto Decrement() -> int;

  [[nodiscard]] auto HoverState() const -> bool;
  auto SetHoverState(bool value) -> void;

  [[nodiscard]] auto Rect() const -> const D2D1_RECT_F&;

  auto Resize(const D2D1_RECT_F& rect) -> void;

private:

  int m_min { 0 };
  int m_max { 10 };
  int m_value { 1 };
  D2D1_RECT_F m_rect { 0, 0, 0, 0 };
  bool m_hover { false };

};

inline setting_slider::setting_slider(int min, int max, int value) : 
  m_min { min }, m_max { max }, m_value { value }
{
}

inline setting_slider::setting_slider(int min, int max, int value, const D2D1_RECT_F& rect) : 
  m_min { min }, m_max { max }, m_value { value }, m_rect { rect }
{
}

[[nodiscard]] inline auto setting_slider::Min() const -> int
{
  return m_min;
}

[[nodiscard]] inline auto setting_slider::Max() const -> int
{
  return m_max;
}

[[nodiscard]] inline auto setting_slider::Value() const -> int
{
  return m_value;
}

[[nodiscard]] inline auto setting_slider::HoverState() const -> bool
{
  return m_hover;
}

inline auto setting_slider::SetHoverState(bool value) -> void
{
  m_hover = value;
}

[[nodiscard]] inline auto setting_slider::Rect() const -> const D2D1_RECT_F&
{
  return m_rect;
}

inline auto setting_slider::Resize(const D2D1_RECT_F& rect) -> void
{
  m_rect = rect;
}

inline auto setting_slider::Increment() -> int
{
  if( ++m_value > m_max )
  {
    m_value = m_max;
  }

  return m_value;
}

inline auto setting_slider::Decrement() -> int
{
  if( --m_value < m_min )
  {
    m_value = m_min;
  }

  return m_value;
}
