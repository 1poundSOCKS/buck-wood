#pragma once

struct window_mouse_data
{
  int x = 0;
  int y = 0;
  bool leftButtonDown = false;
  bool rightButtonDown = false;
};

struct window_data
{
  WORD width = 0;
  WORD height = 0;
  RECT clientRect = {0, 0, 0, 0};
  window_mouse_data mouse;
};
