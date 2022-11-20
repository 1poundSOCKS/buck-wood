#ifndef _control_state_
#define _control_state_

#include "framework.h"

struct control_state
{
  void Refresh(const input_state& inputState, const input_state& previousInputState);

  bool escapeKeyDown = false;
  bool escapeKeyPress = false;
  bool spacebarKeyDown = false;
  bool spacebarKeyPress = false;
  bool deleteKeyPress = false;
  bool functionKey_1 = false;
  bool f1Press = false;
  bool f2Press = false;
  bool f12Press = false;
  bool keyPress_y = false;
  bool keyPress_n = false;

  bool leftMouseButtonDown = false;
  bool leftMouseButtonReleased = false;
  bool rightMouseButtonDown = false;
  bool rightMouseButtonReleased = false;
  render_target_mouse_data renderTargetMouseData;
  bool leftMouseButtonDrag = false;
  bool rightMouseButtonDrag = false;
};

#endif
