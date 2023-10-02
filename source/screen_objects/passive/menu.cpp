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
  auto selectedButton = GetSelectedButton();

  if( controlData.Down() )
  {
    SelectNextButton(selectedButton);
  }
  else if( controlData.Up() )
  {
    SelectPreviousButton(selectedButton);
  }

  for( auto& button : m_buttons )
  {
    button.Update(controlData);
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
