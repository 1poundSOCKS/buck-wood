#ifndef _main_menu_screen_state_
#define _main_menu_screen_state_

#include "main_window.h"
#include "screen_render_data.h"

struct main_menu_screen_state
{
  main_menu_screen_state(screen_render_data renderData);

  screen_render_data renderData;

  enum view_state { view_default, view_exit };
  view_state viewState = view_default;

  render_target_mouse_data renderTargetMouseData;

  bool starting = true;
  bool running = true;
  bool quit = false;
  bool startPlay = false;
  bool startLevelEdit = false;
  bool saveGameLevelData = false;
  bool checkSaveOnExit = false;
  float mousePointerX = 0;
  float mousePointerY = 0;
};

#endif
