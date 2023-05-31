#pragma once

#include "object_input_data.h"
#include "button.h"

class menu
{
public:

  using button_collection = std::vector<button>;
  using callback_for_hidden_flag = std::function<bool()>;

  menu();
  menu(const menu& menuToCopy);

  auto SetCallbackForHiddenFlag(callback_for_hidden_flag callbackForHidden) -> void;
  auto AddButton(button&& menuButton) -> void;

  auto Update(const object_input_data& inputData) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:
  
  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  auto UpdateAllButtonCallbacks() -> void;
  auto UpdateButtonCallbacks(button& buttonToUpdate) -> void;
  
  callback_for_hidden_flag m_callbackForHiddenFlag = [](){ return false; };
  bool m_hidden = false;
  button_collection m_buttons;

};
