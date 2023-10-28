#pragma once

#include "menu_def.h"

class menu_controller
{

public:

  auto Open(const menu_def& def) -> void;
  auto Close() -> void;
  [[nodiscard]] auto GetCurrent() -> menu&;

private:

  using menu_collection = std::stack<menu>;
  menu_collection m_menus;

};

[[nodiscard]] inline auto menu_controller::GetCurrent() -> menu&
{
  return m_menus.top();
}
