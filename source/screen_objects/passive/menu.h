#pragma once

#include "menu_control_data.h"
#include "button.h"

class menu
{
public:

  using button_collection = std::vector<button>;
  using button_selection_type = int;

  menu();
  menu(const menu& menuToCopy);

  auto AddButton(button&& menuButton) -> void;
  auto SelectFirstButton() -> void;
  auto Update(const menu_control_data& controlData) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  [[nodiscard]] auto GetSelectedButton() const -> button_selection_type;
  auto SelectNextButton(button_selection_type currentButton) -> void;
  auto SelectPreviousButton(button_selection_type currentButton) -> void;
  [[nodiscard]] auto ValidButton(button_selection_type selectedButton) const -> bool;
  
  button_collection m_buttons;

};

inline auto menu::SelectFirstButton() -> void
{
  for( auto& button : m_buttons )
  {
    button.SetHoverState(false);
  }

  if( m_buttons.size() > 0 )
  {
    m_buttons.front().SetHoverState(true);
  }  
}

[[nodiscard]] inline auto menu::GetSelectedButton() const -> button_selection_type
{
  auto selectedButton = -1;

  for( auto currentButton = 0; selectedButton == - 1 && currentButton < m_buttons.size(); ++currentButton )
  {
    if( m_buttons[currentButton].GetHoverState() )
    {
      selectedButton = currentButton;
    }
  }

  return selectedButton;
}

inline auto menu::SelectNextButton(button_selection_type currentButton) -> void
{
  auto selectedButton = currentButton + 1;

  if( ValidButton(selectedButton) )
  {
    m_buttons[selectedButton].SetHoverState(true);

    if( ValidButton(currentButton) )
    {
      m_buttons[currentButton].SetHoverState(false);
    }
  }
}

inline auto menu::SelectPreviousButton(button_selection_type currentButton) -> void
{
  auto selectedButton = currentButton - 1;

  if( ValidButton(selectedButton) )
  {
    m_buttons[selectedButton].SetHoverState(true);

    if( ValidButton(currentButton) )
    {
      m_buttons[currentButton].SetHoverState(false);
    }
  }
}

[[nodiscard]] inline auto menu::ValidButton(button_selection_type selectedButton) const -> bool
{
  return selectedButton > - 1 && selectedButton < m_buttons.size() ? true : false;
}
