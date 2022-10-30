#ifndef _control_state_
#define _control_state_

#include "framework/framework.h"

struct control_state
{
  bool quit = false;
  bool quitPress = false;
  bool startGame = false;
  bool functionKey_1 = false;
  bool accelerate = false;
  bool shoot = false;
  float mouseX = 0, mouseY = 0;
  float renderTargetMouseX = 0, renderTargetMouseY = 0;
  float worldMouseX = 0, worldMouseY = 0;
};

void RefreshControlState(control_state& controlState, const d2d_app& app, const D2D1::Matrix3x2F& worldViewTransform);

#endif
