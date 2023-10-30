#pragma once

#include "button_def.h"
#include "menu.h"

class menu_def
{

public:

  using button_defs_collection = std::vector<button_def>;

  menu_def(D2D1_RECT_F rect);

  auto AddButtonDef(button_def&& buttonDef) -> void;
  
  [[nodiscard]] auto GetButtonDefs() const -> const button_defs_collection&;
  [[nodiscard]] auto CreateMenu() const -> menu;

private:

  auto UpdateButtons() -> void;
  [[nodiscard]] auto GetButtonRect(size_t buttonIndex) const -> D2D1_RECT_F;

  D2D1_RECT_F m_rect = { 0, 0, 0, 0 };
  button_defs_collection m_buttonDefs;
};
