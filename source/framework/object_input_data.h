#pragma once

#include "main_window.h"

class object_input_data
{
public:

  struct mouse_data
  {
    float x;
    float y;
    bool leftButtonDown;
    bool rightButtonDown;
  };

  object_input_data();

  auto SetMouseData(const mouse_data& mouseData) -> void;
  auto GetMouseData() const -> const mouse_data&;

private:

  mouse_data m_mouseData;
};
