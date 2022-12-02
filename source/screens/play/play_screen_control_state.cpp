#include "pch.h"
#include "play_screen_control_state.h"

void RefreshControlState(play_screen_control_state& controlState, const control_state& baseControlState)
{
  controlState.pausePlay = baseControlState.escapeKeyPress;
  controlState.returnToMenu = baseControlState.escapeKeyPress;
  controlState.restartPlay = baseControlState.spacebarKeyPress;
  RefreshControlState(controlState.levelControlState, baseControlState);
}
