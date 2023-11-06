#include "pch.h"
#include "menu_def.h"
#include "column_def.h"

menu_def::menu_def(D2D1_RECT_F rect) : m_rect(rect)
{
}

auto menu_def::AddButtonDef(button_def&& buttonDef) -> void
{
  m_buttonDefs.emplace_back(buttonDef);
  UpdateButtons();
}

auto menu_def::UpdateButtons() -> void
{
  column_def columnDefs { m_rect, m_buttonDefs.size() };

  auto columnDef = std::begin(columnDefs);
  
  for( auto& buttonDef : m_buttonDefs )
  {
    const auto& columnRect = *columnDef;
    buttonDef.SetRect(columnRect);
    ++columnDef;
  }
}

[[nodiscard]] auto menu_def::GetButtonDefs() const -> const button_defs_collection&
{
  return m_buttonDefs;
}

[[nodiscard]] auto menu_def::CreateMenu() const -> menu
{
  menu newMenu;

  for( auto& buttonDef : m_buttonDefs )
  {
    newMenu.AddItem(buttonDef.CreateButton(), false);
  }

  return newMenu;
}
