#pragma once

#include "menu_control_data.h"

class button
{
public:

  using callback_for_flag = std::function<bool()>;

  button(D2D1_RECT_F rect, LPCWSTR text, std::function<void()> m_eventClicked);

  auto SetCallbackForHiddenFlag(callback_for_flag callbackForHiddenFlag) -> void;
  auto SetCallbackForEnabledFlag(callback_for_flag callbackForEnabledFlag) -> void;
  
  [[nodiscard]] auto GetHoverState() const -> bool;
  auto SetHoverState(bool value) -> void;
  
  auto Click() -> void;
  
  [[nodiscard]] auto Rect() const -> const D2D1_RECT_F&;
  [[nodiscard]] auto Text() const -> const std::wstring&;

  auto Update() -> void;

private:

  [[nodiscard]] static auto IsInsideRect(float x, float y, D2D1_RECT_F rect) -> bool;

  std::wstring m_text = L"";
  float m_buttonHeight = 0;
  D2D1_RECT_F m_rect = { 0, 0, 0, 0 };
  std::function<void()> m_eventClicked = [](){};

  bool m_hidden { false };
  callback_for_flag m_callbackForHiddenFlag = [](){ return false; };

  bool m_enabled { true };
  callback_for_flag m_callbackForEnabledFlag = [](){ return true; };  

  bool m_hover { false };
};

inline auto button::SetHoverState(bool value) -> void
{
  m_hover = value;
}

inline auto button::Click() -> void
{
  if( !m_hidden && m_enabled && m_hover )
  {
    m_eventClicked();
  }
}

[[nodiscard]] inline auto button::Rect() const -> const D2D1_RECT_F&
{
  return m_rect;
}

[[nodiscard]] inline auto button::Text() const -> const std::wstring&
{
  return m_text;
}
