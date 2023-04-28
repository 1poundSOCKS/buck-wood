#pragma once

#include "object_input_data.h"
#include "button.h"

class menu
{
public:

  using button_collection = std::vector<button>;

  auto AddButton(button&& menuButton) -> void;

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  auto Update(const object_input_data& inputData, int64_t clockCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:
  
  button_collection m_buttons;

};
