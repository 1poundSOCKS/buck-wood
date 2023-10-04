#include "pch.h"
#include "menu.h"

menu::menu()
{
}

menu::menu(const menu& menuToCopy)
{
  m_buttons = menuToCopy.m_buttons;
}

auto menu::AddButton(button&& menuButton) -> void
{
  m_buttons.emplace_back(menuButton);
}

auto menu::Update(const menu_control_data& controlData) -> void
{
  for( auto& button : m_buttons )
  {
    button.Update();
  }

  auto selectedButton = GetSelectedButton();

  if( ValidButton(selectedButton) )
  {
    if( controlData.Down() )
    {
      SelectNextButton(selectedButton);
    }
    else if( controlData.Up() )
    {
      SelectPreviousButton(selectedButton);
    }
    else if( controlData.Select() )
    {
      m_buttons[selectedButton].Click();
    }
  }
}

auto menu::Render(D2D1_RECT_F viewRect) const -> void
{
  for( const auto& button : m_buttons )
  {
    if( !button.GetHoverState() )
      button.Render(viewRect);
  }

  for( const auto& button : m_buttons )
  {
    if( button.GetHoverState() )
      button.Render(viewRect);
  }
}
