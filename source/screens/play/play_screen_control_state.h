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

void ReadControlState(const screen_input_state& inputState, play_screen_control_state& controlState);

#endif
