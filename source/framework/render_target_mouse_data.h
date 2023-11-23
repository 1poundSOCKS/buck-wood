#pragma once

#include "window_data.h"

struct render_target_mouse_data
{
  render_target_mouse_data() = default;
  render_target_mouse_data(const window_data& windowData, const D2D1_SIZE_F& renderTargetSize);

  float x = 0;
  float y = 0;
  D2D1_SIZE_F size;
};

inline render_target_mouse_data::render_target_mouse_data(const window_data& windowData, const D2D1_SIZE_F& renderTargetSize)
{
  long clientWidth = windowData.clientRect.right - windowData.clientRect.left;
  long clientHeight = windowData.clientRect.bottom - windowData.clientRect.top;
  x = static_cast<float>(windowData.mouse.x * renderTargetSize.width) / static_cast<float>(clientWidth);
  y = static_cast<float>(windowData.mouse.y * renderTargetSize.height) / static_cast<float>(clientHeight);
}
