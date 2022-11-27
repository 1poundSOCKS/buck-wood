#ifndef _main_render_window_
#define _main_render_window_

#include "render.h"

struct main_render_window
{
  main_render_window(HWND windowHandle, UINT fps);

  HWND windowHandle;
  render_target renderTarget;
};

#endif
