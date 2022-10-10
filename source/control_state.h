#ifndef _control_state_
#define _control_state_

#include "d2d_app.h"

struct control_state
{
  control_state() : quit(false), quitPress(false), startGame(false), left(false), right(false), accelerate(false), shoot(false), mouseX(0), mouseY(0) {}
  
  bool quit;
  bool quitPress;
  bool startGame;
  bool left;
  bool right;
  bool accelerate;
  bool shoot;
  float mouseX, mouseY;
};

std::unique_ptr<control_state> GetControlState(const d2d_app& app, const control_state& previousControlState);

#endif
