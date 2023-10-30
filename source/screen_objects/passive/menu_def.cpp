#include "pch.h"
#include "menu_def.h"

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
  size_t buttonIndex = 0;
  
  for( auto& buttonDef : m_buttonDefs )
  {
    buttonDef.SetRect(GetButtonRect(buttonIndex++));
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
    newMenu.AddButton(buttonDef.CreateButton());
  }

  return newMenu;
}

[[nodiscard]] auto menu_def::GetButtonRect(size_t buttonIndex) const -> D2D1_RECT_F
{
  auto buttonCount = m_buttonDefs.size();

  auto relativeDividerHeight = 1;
  auto relativeButtonHeight = 10;
  auto sumOfRelativeHeights = relativeButtonHeight * buttonCount + relativeDividerHeight * ( buttonCount - 1 );

  auto menuHeight = m_rect.bottom - m_rect.top;

  auto buttonHeight = menuHeight * relativeButtonHeight / sumOfRelativeHeights;
  auto dividerHeight = menuHeight * relativeDividerHeight / sumOfRelativeHeights;
  auto buttonAndDividerHeight = buttonHeight + dividerHeight;
  
  auto buttonTop = m_rect.top + buttonIndex * buttonAndDividerHeight;
  auto buttonBottom = buttonTop + buttonHeight;

  return { m_rect.left, buttonTop, m_rect.right, buttonBottom };
}
