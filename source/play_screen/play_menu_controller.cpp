#include "pch.h"
#include "play_menu_controller.h"

auto play_menu_controller::OpenRoot() -> void
{
  Open();
  GetCurrent().AddItem<button>(L"Resume", [this]() -> void { m_selection = selection::resume; });
  GetCurrent().AddItem<button>(L"Settings", [this]() -> void { OpenSettingsMenu(); });
  GetCurrent().AddItem<button>(L"Quit", [this]() -> void { m_selection = selection::quit; });
  GetCurrent().ResizeItems();
  GetCurrent().SelectFirstItem();
}
