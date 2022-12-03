#ifndef _play_screen_control_state_
#define _play_screen_control_state_

#include "level_state.h"
#include "input_state.h"
#include "main_window.h"

struct play_screen_control_state
{
  bool returnToMenu = false;
  bool restartPlay = false;
  bool pausePlay = false;
  level_control_state levelControlState;
};

struct play_screen_control_state_reader
{
  const window_data& windowData;
  const window_data& previousWindowData;
  const keyboard_state& keyboardState;
  const keyboard_state& previousKeyboardState;

  void Read(play_screen_control_state& controlState) const
  {
    controlState.levelControlState.renderTargetMouseData.x = windowData.mouse.x;
    controlState.levelControlState.renderTargetMouseData.y = windowData.mouse.y;
    controlState.levelControlState.renderTargetMouseData.size.width = windowData.width;
    controlState.levelControlState.renderTargetMouseData.size.height = windowData.height;
  }
};

void RefreshControlState(play_screen_control_state& controlState, const control_state& baseControlState);
void ReadControlState(ID2D1RenderTarget* renderTarget, window_data& windowData, keyboard_state& keyboard, play_screen_control_state& controlState);

#endif
