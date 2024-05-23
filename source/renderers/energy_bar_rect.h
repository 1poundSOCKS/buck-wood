#pragma once

#include "framework.h"

class energy_bar_rect
{

public:

  energy_bar_rect(RECT_F objectRect)
  {
    auto left = objectRect.left;
    auto right = objectRect.right;
    auto width = right - left;
    auto bottom = objectRect.top - 20;
    auto top = bottom - width / 8;
    m_value = { left, top, right, bottom };
  }

  [[nodiscard]] auto Get() const noexcept -> RECT_F
  {
    return m_value;
  }

private:

  RECT_F m_value;

};
