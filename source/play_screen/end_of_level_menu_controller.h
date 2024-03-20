#pragma once

#include "common_menu_controller.h"

class end_of_level_menu_controller : public common_menu_controller
{
public:

  enum class selection { NONE, CONTINUE };

  auto OpenRoot() -> void;
  [[nodiscard]] auto Selection() -> selection;

private:

  selection m_selection { selection::NONE };

};

[[nodiscard]] inline auto end_of_level_menu_controller::Selection() -> selection
{
  return std::exchange(m_selection, selection::NONE);
}

inline auto end_of_level_menu_controller::OpenRoot() -> void
{
  Open();
  GetCurrent().AddButton(L"Continue", [this]() -> void { m_selection = selection::CONTINUE; });
  GetCurrent().ResizeItems();
  GetCurrent().SelectFirstItem();
}
