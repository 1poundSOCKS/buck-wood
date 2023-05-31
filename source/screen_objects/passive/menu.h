#pragma once

#include "object_input_data.h"
#include "button.h"

class menu
{
public:

  using button_collection = std::vector<button>;

  menu();
  menu(const menu& menuToCopy);

  auto AddButton(button&& menuButton) -> void;
  auto Update(const object_input_data& inputData) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:
  
  button_collection m_buttons;

};
