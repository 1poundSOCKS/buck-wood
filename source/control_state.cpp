#include "pch.h"
#include "control_state.h"

bool MouseHasMoved(const input_state& inputState, const input_state& previousInputState);

void control_state::Refresh(const input_state& inputState, const input_state& previousInputState)
{
  const auto& keyboardState = inputState.keyboardState;
  const auto& previousKeyboardState = previousInputState.keyboardState;

  escapeKeyDown = keyboardState.data[DIK_ESCAPE] & 0x80;
  escapeKeyPress = !(keyboardState.data[DIK_ESCAPE] & 0x80) && previousKeyboardState.data[DIK_ESCAPE] & 0x80;
  spacebarKeyDown = keyboardState.data[DIK_SPACE] & 0x80;
  spacebarKeyPress = !(keyboardState.data[DIK_SPACE] & 0x80) && previousKeyboardState.data[DIK_SPACE] & 0x80;
  deleteKeyPress = !(keyboardState.data[DIK_DELETE] & 0x80) && previousKeyboardState.data[DIK_DELETE] & 0x80;
  
  functionKey_1 = keyboardState.data[DIK_F1] & 0x80;
  
  f1Press = !(keyboardState.data[DIK_F1] & 0x80) && previousKeyboardState.data[DIK_F1] & 0x80;
  f2Press = !(keyboardState.data[DIK_F2] & 0x80) && previousKeyboardState.data[DIK_F2] & 0x80;
  f12Press = !(keyboardState.data[DIK_F12] & 0x80) && previousKeyboardState.data[DIK_F12] & 0x80;

  keyPress_y = !(keyboardState.data[DIK_Y] & 0x80) && previousKeyboardState.data[DIK_Y] & 0x80;
  keyPress_n = !(keyboardState.data[DIK_N] & 0x80) && previousKeyboardState.data[DIK_N] & 0x80;

  keyPress_t = !(keyboardState.data[DIK_T] & 0x80) && previousKeyboardState.data[DIK_T] & 0x80;
  keyPress_o = !(keyboardState.data[DIK_O] & 0x80) && previousKeyboardState.data[DIK_O] & 0x80;

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
