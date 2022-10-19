#ifndef _control_state_
#define _control_state_

#include "d2d_app.h"

struct control_state
{
  control_state();
  
  bool quit;
  bool quitPress;
  bool startGame;
  bool functionKey_1 = false;
  bool left;
  bool right;
  bool accelerate;
  bool shoot;
  float mouseX, mouseY;
  float renderTargetMouseX, renderTargetMouseY;
  float gameMouseX, gameMouseY;
};

std::unique_ptr<control_state> GetControlState(const d2d_app& app, const control_state& previousControlState);

#endif
