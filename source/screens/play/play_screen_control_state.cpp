#include "pch.h"
#include "play_screen_control_state.h"

void ReadControlState(const screen_input_state& inputState, play_screen_control_state& controlState)
{
  controlState.pausePlay = false;
  controlState.returnToMenu = false;
  controlState.restartPlay = false;

  controlState.levelControlState.renderTargetMouseData.x = inputState.windowData.mouse.x;
  controlState.levelControlState.renderTargetMouseData.y = inputState.windowData.mouse.y;
  controlState.levelControlState.renderTargetMouseData.size.width = inputState.windowData.width;
  controlState.levelControlState.renderTargetMouseData.size.height = inputState.windowData.height;

  controlState.levelControlState.thrust = inputState.windowData.mouse.rightButtonDown;
  controlState.levelControlState.shoot = inputState.windowData.mouse.leftButtonDown;
}
