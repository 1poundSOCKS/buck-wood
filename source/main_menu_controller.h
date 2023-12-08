#pragma once

#include "common_menu_controller.h"

class main_menu_controller : public common_menu_controller
{

public:

  enum class selection { none, start, exit };

  auto OpenRoot() -> void;
  [[nodiscard]] auto Selection() -> selection;

private:

  selection m_selection { selection::none };

};

[[nodiscard]] inline auto main_menu_controller::Selection() -> selection
{
  return std::exchange(m_selection, selection::none);
}
