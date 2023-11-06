#include "pch.h"
#include "button.h"

button::button(LPCWSTR text) : m_text { text }
{
}

button::button(LPCWSTR text, std::function<void()> eventClicked) : 
  m_text { text },
  m_eventClicked { eventClicked }
{
}

button::button(LPCWSTR text, std::function<void()> eventClicked, D2D1_RECT_F rect) : 
  m_text { text },
  m_eventClicked { eventClicked },
  m_rect { rect }
{
}

[[nodiscard]] auto button::IsInsideRect(float x, float y, D2D1_RECT_F rect) -> bool
{
  return ( x < rect.left || x > rect.right || y < rect.top || y > rect.bottom ) ? false : true;
}
