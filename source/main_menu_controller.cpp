#include "pch.h"
#include "main_menu_controller.h"
#include "button.h"

auto main_menu_controller::OpenRoot() -> void
{
  Open();
  GetCurrent().AddItem<button>(L"Start", [this]() -> void { m_selection = selection::start; });
  GetCurrent().AddItem<button>(L"Settings", [this]() -> void { OpenSettingsMenu(); });
  GetCurrent().AddItem<button>(L"Exit", [this]() -> void { m_selection = selection::exit; });
  GetCurrent().ResizeItems();
  GetCurrent().SelectFirstItem();
}
