#include "pch.h"
#include "button_def.h"

button_def::button_def(LPCWSTR text) : 
  m_text(text)
{
  m_eventClicked = []() -> void {};
}

button_def::button_def(LPCWSTR text, D2D1_RECT_F rect, std::function<void()> eventClicked) : 
  m_text(text), m_rect(rect), m_eventClicked(eventClicked)
{

}

button_def::button_def(LPCWSTR text, std::function<void()> eventClicked) : 
  m_text(text), m_eventClicked(eventClicked)
{

}

auto button_def::SetRect(D2D1_RECT_F rect) -> void
{
  m_rect = rect;
}

[[nodiscard]] auto button_def::CreateButton() const -> button
{
  return button(m_rect, m_text.c_str(), m_eventClicked);
}
