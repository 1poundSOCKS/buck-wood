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

auto menu::SelectFirstButton() -> void
{
  
}

auto menu::Update(const object_input_data& inputData) -> void
{
  if( inputData.GamepadAttached() && m_buttons.size() )
  {
    auto selectedButton = GetSelectedButton();

    if( inputData.DownClicked() )
    {
      SelectNextButton(selectedButton);
    }
    else if( inputData.UpClicked() )
    {
      SelectPreviousButton(selectedButton);
    }
  }

  for( auto& button : m_buttons )
  {
    button.Update(inputData);
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
