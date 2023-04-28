#include "pch.h"
#include "button_def.h"

button_def::button_def(D2D1_RECT_F rect, LPCWSTR text, std::function<void()> eventClicked, bool hidden) : 
  m_rect(rect), m_text(text), m_eventClicked(eventClicked), m_hidden(hidden)
{

}

auto button_def::CreateButton() -> button
{
  return button(m_rect, m_text.c_str(), m_eventClicked, m_hidden);
}
