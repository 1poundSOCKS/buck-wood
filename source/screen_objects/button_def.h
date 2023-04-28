#pragma once

#include "button.h"

class button_def
{

public:

  button_def(D2D1_RECT_F rect, LPCWSTR text, std::function<void()> eventClicked, bool hidden = false);

  auto CreateButton() -> button;

private:

  D2D1_RECT_F m_rect;
  std::wstring m_text;
  std::function<void()> m_eventClicked;
  bool m_hidden = false;

};
