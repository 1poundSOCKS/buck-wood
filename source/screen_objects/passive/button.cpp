#include "pch.h"
#include "button.h"

button::button(D2D1_RECT_F rect, LPCWSTR text, std::function<void()> eventClicked) : 
  m_rect { rect },
  m_buttonHeight { rect.bottom - rect.top },
  m_text { text }, m_eventClicked { eventClicked }
{
}

auto button::SetCallbackForHiddenFlag(callback_for_flag callbackForHiddenFlag) -> void
{
  m_callbackForHiddenFlag = callbackForHiddenFlag;
}

auto button::SetCallbackForEnabledFlag(callback_for_flag callbackForEnabledFlag) -> void
{
  m_callbackForEnabledFlag = callbackForEnabledFlag;
}

auto button::GetHoverState() const -> bool
{
  return m_hover;
}

auto button::Update() -> void
{
  m_hidden = m_callbackForHiddenFlag();
  m_enabled = m_callbackForEnabledFlag();
}

[[nodiscard]] auto button::IsInsideRect(float x, float y, D2D1_RECT_F rect) -> bool
{
  return ( x < rect.left || x > rect.right || y < rect.top || y > rect.bottom ) ? false : true;
}
