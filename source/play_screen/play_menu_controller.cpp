#include "pch.h"
#include "play_menu_controller.h"

auto play_menu_controller::OpenRoot() -> void
{
  Open();
  GetCurrent().AddButton(L"Resume", [this]() -> void { m_selection = selection::resume; });
  GetCurrent().AddButton(L"Settings", [this]() -> void { OpenSettingsMenu(); });
  GetCurrent().AddButton(L"Quit", [this]() -> void { m_selection = selection::quit; });
  GetCurrent().ResizeItems();
  GetCurrent().SelectFirstItem();
}
