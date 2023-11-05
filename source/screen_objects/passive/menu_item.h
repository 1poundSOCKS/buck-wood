#pragma once

#include "button.h"
#include "menu_slider.h"

class menu_item
{

public:

  enum class type { button = 0, slider };

  menu_item(const button& item);
  menu_item(const menu_slider& item);

  [[nodiscard]] auto Type() const -> type;
  [[nodiscard]] auto Button() const -> const button&;
  [[nodiscard]] auto Slider() const -> const menu_slider&;

  [[nodiscard]] auto GetHoverState() const -> bool;
  auto SetHoverState(bool value) -> void;

  auto Update() -> void;
  auto Click() -> void;

private:

  type m_type;
  using item_type = std::variant<button, menu_slider>;
  item_type m_item;

};

inline menu_item::menu_item(const button& item) : m_type { type::button }, m_item { item }
{
}

inline menu_item::menu_item(const menu_slider& item) : m_type { type::slider }, m_item { item }
{
}

[[nodiscard]] inline auto menu_item::Type() const -> type
{
  return m_type;
}

[[nodiscard]] inline auto menu_item::Button() const -> const button&
{
  return *std::get_if<button>(&m_item);
}

[[nodiscard]] inline auto menu_item::Slider() const -> const menu_slider&
{
  return *std::get_if<menu_slider>(&m_item);
}

inline auto menu_item::GetHoverState() const -> bool
{
  bool hoverState = false;

  switch( m_type )
  {
    case type::button:
      hoverState = std::get_if<button>(&m_item)->GetHoverState();
      break;
  }

  return hoverState;
}

inline auto menu_item::SetHoverState(bool value) -> void
{
  switch( m_type )
  {
    case type::button:
      std::get_if<button>(&m_item)->SetHoverState(value);
      break;
  }
}

inline auto menu_item::Update() -> void
{
  switch( m_type )
  {
    case type::button:
      std::get_if<button>(&m_item)->Update();
      break;
  }
}

inline auto menu_item::Click() -> void
{
  switch( m_type )
  {
    case type::button:
      std::get_if<button>(&m_item)->Click();
      break;
  }
}
