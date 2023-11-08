#pragma once

#include "button.h"
#include "setting_slider.h"

class menu_item
{

public:

  using item_type = std::variant<button, setting_slider>;

  menu_item(const button& item);
  menu_item(const setting_slider& item);

  [[nodiscard]] auto Get() const -> const item_type&;

  [[nodiscard]] auto HoverState() const -> bool;
  auto SetHoverState(bool value) -> void;

  auto Click() -> void;
  auto Update(const menu_control_data& controlData) -> void;

  auto Resize(const D2D1_RECT_F& rect) -> void;

private:

  item_type m_item;

};

inline menu_item::menu_item(const button& item) : m_item { item }
{
}

inline menu_item::menu_item(const setting_slider& item) : m_item { item }
{
}

[[nodiscard]] inline auto menu_item::Get() const -> const item_type&
{
  return m_item;
}
