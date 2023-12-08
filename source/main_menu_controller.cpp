#include "pch.h"
#include "main_menu_controller.h"
#include "button.h"

auto main_menu_controller::OpenRoot() -> void
{
  Open();
  GetCurrent().AddButton(L"Start", [this]() -> void { m_selection = selection::start; });
  GetCurrent().AddButton(L"Settings", [this]() -> void { OpenSettingsMenu(); });
  GetCurrent().AddButton(L"Exit", [this]() -> void { m_selection = selection::exit; });
  GetCurrent().ResizeItems();
  GetCurrent().SelectFirstItem();
}
