#include "pch.h"
#include "setting_slider.h"

setting_slider::setting_slider(int min, int max, int value) : 
  m_min { min }, m_max { max }, m_value { value }
{
}

setting_slider::setting_slider(int min, int max, int value, const D2D1_RECT_F& rect) : 
  m_min { min }, m_max { max }, m_value { value }, m_rect { rect }
{
}

[[nodiscard]] auto setting_slider::Min() const -> int
{
  return m_min;
}

[[nodiscard]] auto setting_slider::Max() const -> int
{
  return m_max;
}

[[nodiscard]] auto setting_slider::Value() const -> int
{
  return m_value;
}

[[nodiscard]] auto setting_slider::HoverState() const -> bool
{
  return m_hover;
}

auto setting_slider::SetHoverState(bool value) -> void
{
  m_hover = value;
}

auto setting_slider::Select() -> void
{

}

auto setting_slider::Unselect() -> void
{
  
}

[[nodiscard]] auto setting_slider::Rect() const -> const D2D1_RECT_F&
{
  return m_rect;
}

auto setting_slider::Resize(const D2D1_RECT_F& rect) -> void
{
  m_rect = rect;
}

auto setting_slider::Increment() -> int
{
  if( ++m_value > m_max )
  {
    m_value = m_max;
  }

  return m_value;
}

auto setting_slider::Decrement() -> int
{
  if( --m_value < m_min )
  {
    m_value = m_min;
  }

  return m_value;
}

[[nodiscard]] auto setting_slider::Name() const -> const std::wstring&
{
  return m_name;
}
