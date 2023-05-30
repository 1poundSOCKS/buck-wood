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

  auto SetPreviousMouseData(const mouse_data& mouseData) -> void;
  auto GetPreviousMouseData() const -> const mouse_data&;

  [[nodiscard]] auto LeftMouseButtonClicked() const -> bool;
  [[nodiscard]] auto RightMouseButtonClicked() const -> bool;

private:

  mouse_data m_mouseData;
  mouse_data m_previousMouseData;
};
