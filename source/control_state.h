#ifndef _control_state_
#define _control_state_

#include "framework/framework.h"

struct control_state
{
  void Refresh(const input_state& inputState, const input_state& previousInputState);

  bool escapeKeyDown = false;
  bool escapeKeyPress = false;
  bool spacebarKeyDown = false;
  bool spacebarKeyPress = false;
  bool functionKey_1 = false;
  bool leftMouseButtonDown = false;
  bool rightMouseButtonDown = false;
  float ratioMouseX = 0, ratioMouseY = 0;
  float renderTargetMouseX = 0, renderTargetMouseY = 0;
  bool leftMouseButtonDrag = false;
  bool rightMouseButtonDrag = false;
};

#endif
