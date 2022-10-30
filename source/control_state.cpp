#include "control_state.h"

void RefreshControlState(control_state& controlState, const d2d_app& app, const D2D1::Matrix3x2F& worldViewTransform)
{
  const auto& keyboardState = app.inputState.keyboardState;
  const auto& previousKeyboardState = app.previousInputState.keyboardState;

  controlState.quit = keyboardState[DIK_ESCAPE] & 0x80;
  controlState.quitPress = !(keyboardState[DIK_ESCAPE] & 0x80) && previousKeyboardState[DIK_ESCAPE] & 0x80;
  controlState.startGame = keyboardState[DIK_SPACE] & 0x80;
  controlState.functionKey_1 = keyboardState[DIK_F1] & 0x80;

  controlState.accelerate = app.inputState.rightMouseButtonDown;
  controlState.shoot = app.inputState.leftMouseButtonDown;

  RECT clientRect;
  GetClientRect(app.wnd, &clientRect);
  controlState.mouseX = static_cast<float>(app.inputState.mouseX) / static_cast<float>(clientRect.right - clientRect.left);
  controlState.mouseY = static_cast<float>(app.inputState.mouseY) / static_cast<float>(clientRect.bottom - clientRect.top);

  const auto renderTargetSize = app.d2d_rendertarget->GetSize();
  controlState.renderTargetMouseX = controlState.mouseX * renderTargetSize.width;
  controlState.renderTargetMouseY = controlState.mouseY * renderTargetSize.height;

  auto mouseTransform = worldViewTransform;
  if( mouseTransform.Invert() )
  {
    D2D1_POINT_2F inPoint;
    inPoint.x = controlState.renderTargetMouseX;
    inPoint.y = controlState.renderTargetMouseY;
    auto outPoint = mouseTransform.TransformPoint(inPoint);
    controlState.worldMouseX = outPoint.x;
    controlState.worldMouseY = outPoint.y;
  }
}
