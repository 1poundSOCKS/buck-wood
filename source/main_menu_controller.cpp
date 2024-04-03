#include "pch.h"
#include "main_menu_controller.h"
#include "button.h"
#include "game_state.h"

auto main_menu_controller::OpenRoot() -> void
{
  Open();
  
  if( game_state::saved() )
  {
    GetCurrent().AddButton(L"Resume", [this]() -> void { m_selection = selection::resume; });
    GetCurrent().AddButton(L"Restart", [this]() -> void { m_selection = selection::start; });
  }
  else
  {
    GetCurrent().AddButton(L"Start", [this]() -> void { m_selection = selection::start; });
  }
  
  GetCurrent().AddButton(L"Settings", [this]() -> void { OpenSettingsMenu(); });
  GetCurrent().AddButton(L"Exit", [this]() -> void { m_selection = selection::exit; });
  GetCurrent().ResizeItems();
  GetCurrent().SelectFirstItem();
}
