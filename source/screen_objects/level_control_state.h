#pragma once

#include "render_target_mouse_data.h"

struct level_control_state
{
  bool thrust = false;
  bool shoot = false;
  render_target_mouse_data renderTargetMouseData;
};
