#include "control_state.h"

bool MouseHasMoved(const input_state& inputState, const input_state& previousInputState);

void control_state::Refresh(const input_state& inputState, const input_state& previousInputState)
{
  const auto& keyboardState = inputState.keyboardState;
  const auto& previousKeyboardState = previousInputState.keyboardState;

  escapeKeyDown = keyboardState[DIK_ESCAPE] & 0x80;
  escapeKeyPress = !(keyboardState[DIK_ESCAPE] & 0x80) && previousKeyboardState[DIK_ESCAPE] & 0x80;
  spacebarKeyDown = keyboardState[DIK_SPACE] & 0x80;
  spacebarKeyPress = !(keyboardState[DIK_SPACE] & 0x80) && previousKeyboardState[DIK_SPACE] & 0x80;
  functionKey_1 = keyboardState[DIK_F1] & 0x80;

  leftMouseButtonDown = inputState.leftMouseButtonDown;
  rightMouseButtonDown = inputState.rightMouseButtonDown;

  ratioMouseX = inputState.ratioMouseX;
  ratioMouseY = inputState.ratioMouseY;

  renderTargetMouseX = inputState.renderTargetMouseX;
  renderTargetMouseY = inputState.renderTargetMouseY;

  if( inputState.leftMouseButtonDown && previousInputState.leftMouseButtonDown && MouseHasMoved(inputState, previousInputState) )
  {
    leftMouseButtonDrag = true;
  }

  if( inputState.rightMouseButtonDown && previousInputState.rightMouseButtonDown && MouseHasMoved(inputState, previousInputState) )
  {
    rightMouseButtonDrag = true;
  }
}

bool MouseHasMoved(const input_state& inputState, const input_state& previousInputState)
{
  return ( previousInputState.clientMouseX != inputState.clientMouseX || previousInputState.clientMouseY != inputState.clientMouseY ) ? true : false;
}
