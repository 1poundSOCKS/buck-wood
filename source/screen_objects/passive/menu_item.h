#pragma once

#include "button.h"
#include "effects_volume_slider.h"
#include "music_volume_slider.h"

class menu_item
{

public:

  using item_type = std::variant<button, effects_volume_slider, music_volume_slider>;

  menu_item(const auto& item);

  [[nodiscard]] auto Get() const -> const item_type&;

  [[nodiscard]] auto HoverState() const -> bool;
  auto SetHoverState(bool value) -> void;

  auto Click() -> void;
  auto Update(const menu_control_data& controlData) -> void;

  auto Resize(const D2D1_RECT_F& rect) -> void;

private:

  item_type m_item;

};

menu_item::menu_item(const auto& item) : m_item { item }
{
}

[[nodiscard]] inline auto menu_item::Get() const -> const item_type&
{
  return m_item;
}
