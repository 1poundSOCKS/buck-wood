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
  deleteKeyPress = !(keyboardState[DIK_DELETE] & 0x80) && previousKeyboardState[DIK_DELETE] & 0x80;
  functionKey_1 = keyboardState[DIK_F1] & 0x80;

  keyPress_y = !(keyboardState[DIK_Y] & 0x80) && previousKeyboardState[DIK_Y] & 0x80;
  keyPress_n = !(keyboardState[DIK_N] & 0x80) && previousKeyboardState[DIK_N] & 0x80;

  leftMouseButtonDown = inputState.leftMouseButtonDown;
  rightMouseButtonDown = inputState.rightMouseButtonDown;

  renderTargetMouseData = inputState.renderTargetMouseData;

  leftMouseButtonReleased = false;
  if( !inputState.leftMouseButtonDown && previousInputState.leftMouseButtonDown )
  {
    leftMouseButtonReleased = true;
  }

  rightMouseButtonReleased = false;
  if( !inputState.rightMouseButtonDown && previousInputState.rightMouseButtonDown )
  {
    rightMouseButtonReleased = true;
  }

  leftMouseButtonDrag = false;
  if( inputState.leftMouseButtonDown && previousInputState.leftMouseButtonDown && MouseHasMoved(inputState, previousInputState) )
  {
    leftMouseButtonDrag = true;
  }

  leftMouseButtonDrag = false;
  if( inputState.rightMouseButtonDown && previousInputState.rightMouseButtonDown && MouseHasMoved(inputState, previousInputState) )
  {
    rightMouseButtonDrag = true;
  }
}

bool MouseHasMoved(const input_state& inputState, const input_state& previousInputState)
{
  return ( previousInputState.clientMouseData.x != inputState.clientMouseData.x || previousInputState.clientMouseData.y != inputState.clientMouseData.y ) ? true : false;
}
