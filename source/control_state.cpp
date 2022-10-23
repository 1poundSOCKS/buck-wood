#include "control_state.h"

control_state::control_state()
: quit(false), quitPress(false), startGame(false), left(false), right(false), accelerate(false), shoot(false), 
  mouseX(0), mouseY(0), renderTargetMouseX(0), renderTargetMouseY(0)
{
}
