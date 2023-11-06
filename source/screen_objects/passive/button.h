#pragma once

#include "menu_control_data.h"

class button
{
public:

  button(LPCWSTR text);
  button(LPCWSTR text, std::function<void()> m_eventClicked, D2D1_RECT_F rect);

  [[nodiscard]] auto HoverState() const -> bool;
  auto SetHoverState(bool value) -> void;
  
  auto Click() -> void;
  
  [[nodiscard]] auto Rect() const -> const D2D1_RECT_F&;
  [[nodiscard]] auto Text() const -> const std::wstring&;

private:

  [[nodiscard]] static auto IsInsideRect(float x, float y, D2D1_RECT_F rect) -> bool;

  std::wstring m_text = L"";
  float m_buttonHeight = 0;
  D2D1_RECT_F m_rect = { 0, 0, 0, 0 };
  std::function<void()> m_eventClicked = [](){};
  bool m_hover { false };
};

inline auto button::HoverState() const -> bool
{
  return m_hover;
}

inline auto button::SetHoverState(bool value) -> void
{
  m_hover = value;
}

inline auto button::Click() -> void
{
  m_eventClicked();
}

[[nodiscard]] inline auto button::Rect() const -> const D2D1_RECT_F&
{
  return m_rect;
}

[[nodiscard]] inline auto button::Text() const -> const std::wstring&
{
  return m_text;
}
