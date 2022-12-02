#ifndef _play_screen_control_state_
#define _play_screen_control_state_

#include "level_state.h"

struct play_screen_control_state
{
  bool returnToMenu = false;
  bool restartPlay = false;
  bool pausePlay = false;
  level_control_state levelControlState;
};

void RefreshControlState(play_screen_control_state& controlState, const control_state& baseControlState);

#endif
