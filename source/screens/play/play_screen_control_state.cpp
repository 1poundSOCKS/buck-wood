#include "pch.h"
#include "play_screen_control_state.h"

void RefreshControlState(play_screen_control_state& controlState, const control_state& baseControlState)
{
  controlState.pausePlay = baseControlState.escapeKeyPress;
  controlState.returnToMenu = baseControlState.escapeKeyPress;
  controlState.restartPlay = baseControlState.spacebarKeyPress;
  RefreshControlState(controlState.levelControlState, baseControlState);
}

void ReadControlState(ID2D1RenderTarget* renderTarget, window_data& windowData, keyboard_state& keyboard, play_screen_control_state& controlState)
{
  controlState.pausePlay = false;
  controlState.returnToMenu = false;
  controlState.restartPlay = false;
  controlState.levelControlState.renderTargetMouseData.size = renderTarget->GetSize();
  controlState.levelControlState.renderTargetMouseData.x = windowData.mouse.x;
  controlState.levelControlState.renderTargetMouseData.y = windowData.mouse.y;
  controlState.levelControlState.shoot = windowData.mouse.leftButtonDown;
  controlState.levelControlState.thrust = windowData.mouse.rightButtonDown;
}
