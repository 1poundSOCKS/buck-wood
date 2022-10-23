#ifndef _control_state_
#define _control_state_

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
};

#endif
