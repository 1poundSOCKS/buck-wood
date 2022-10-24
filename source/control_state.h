#ifndef _control_state_
#define _control_state_

struct control_state
{
  bool quit = false;
  bool quitPress = false;
  bool startGame = false;
  bool functionKey_1 = false;
  bool left = false;
  bool right = false;
  bool accelerate = false;
  bool shoot = false;
  float mouseX = 0, mouseY = 0;
  float renderTargetMouseX = 0, renderTargetMouseY = 0;
};

#endif
