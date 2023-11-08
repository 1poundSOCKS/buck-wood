#pragma once

#include "common_menu_controller.h"

class play_menu_controller : public common_menu_controller
{
public:

  enum class selection { none, resume, quit };

  auto OpenRoot(const render_target_area& area) -> void;
  [[nodiscard]] auto Selection() -> selection;

private:

  [[nodiscard]] auto GetRootMenuDef(const render_target_area& area) -> menu_def;

  selection m_selection { selection::none };

};

[[nodiscard]] inline auto play_menu_controller::Selection() -> selection
{
  return std::exchange(m_selection, selection::none);
}
