#pragma once

#include "object_input_data.h"
#include "button.h"

class menu
{
public:

  using button_collection = std::vector<button>;

  menu();
  menu(const menu& menuToCopy);

  auto AddButton(button&& menuButton) -> void;
  auto Update(const object_input_data& inputData) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  [[nodiscard]] auto GetSelectedButton() const -> button_collection::size_type;
  auto SelectNextButton(button_collection::size_type ) -> void;
  auto SelectPreviousButton(button_collection::size_type ) -> void;
  
  button_collection m_buttons;

};

[[nodiscard]] inline auto menu::GetSelectedButton() const -> button_collection::size_type
{
  auto selectedButton = -1;

  auto targetView = m_buttons | std::ranges::views::filter([](const auto& currentButton)
  {
    return currentButton.GetHoverState();
  });

  return selectedButton;
}

inline auto menu::SelectNextButton(button_collection::size_type selectedButton) -> void
{

}

inline auto menu::SelectPreviousButton(button_collection::size_type selectedButton) -> void
{

}
