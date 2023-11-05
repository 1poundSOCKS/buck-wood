#include "pch.h"
#include "button.h"

button::button(D2D1_RECT_F rect, LPCWSTR text, std::function<void()> eventClicked) : 
  m_rect { rect },
  m_buttonHeight { rect.bottom - rect.top },
  m_text { text }, m_eventClicked { eventClicked }
{
}

[[nodiscard]] auto button::IsInsideRect(float x, float y, D2D1_RECT_F rect) -> bool
{
  return ( x < rect.left || x > rect.right || y < rect.top || y > rect.bottom ) ? false : true;
}
