#pragma once

#include "button.h"

class button_def
{

public:

  button_def(LPCWSTR text, bool hidden);
  button_def(LPCWSTR text, bool hidden, std::function<void()> eventClicked);
  button_def(LPCWSTR text, bool hidden, D2D1_RECT_F rect, std::function<void()> eventClicked);

  auto SetRect(D2D1_RECT_F rect) -> void;
  [[nodiscard]] auto CreateButton() const -> button;

private:

  D2D1_RECT_F m_rect = { 0, 0, 0, 0 };
  std::wstring m_text;
  std::function<void()> m_eventClicked;
  bool m_hidden = false;

};
