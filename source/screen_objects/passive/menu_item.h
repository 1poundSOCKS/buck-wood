#pragma once

#include "button.h"
#include "menu_slider.h"

class menu_item
{

public:

  using item_type = std::variant<button, menu_slider>;

  menu_item(const button& item);
  menu_item(const menu_slider& item);

  [[nodiscard]] auto Get() const -> const item_type&;

  [[nodiscard]] auto GetHoverState() const -> bool;
  auto SetHoverState(bool value) -> void;

  auto Update() -> void;
  auto Click() -> void;

private:

  item_type m_item;

};

inline menu_item::menu_item(const button& item) : m_item { item }
{
}

inline menu_item::menu_item(const menu_slider& item) : m_item { item }
{
}

[[nodiscard]] inline auto menu_item::Get() const -> const item_type&
{
  return m_item;
}

struct menu_item_visitor_get_hover_state
{
    void operator()(const button& item)
    {
      m_hoverState = item.GetHoverState();
    }

    void operator()(const menu_slider& item)
    {
    }

    bool m_hoverState { false };
};

inline auto menu_item::GetHoverState() const -> bool
{
  menu_item_visitor_get_hover_state visitor {};
  std::visit(visitor, m_item);
  return visitor.m_hoverState;
}

struct menu_item_visitor_set_hover_state
{
    menu_item_visitor_set_hover_state(bool hoverState) : m_hoverState { hoverState }
    {
    }

    void operator()(button& item)
    {
      item.SetHoverState(m_hoverState);
    }

    void operator()(menu_slider& item)
    {
    }

    bool m_hoverState { false };
};

inline auto menu_item::SetHoverState(bool value) -> void
{
  std::visit(menu_item_visitor_set_hover_state { value }, m_item);
}

struct menu_item_visitor_update
{
    void operator()(button& item)
    {
      item.Update();
    }

    void operator()(menu_slider& item)
    {
    }
};

inline auto menu_item::Update() -> void
{
  std::visit(menu_item_visitor_update {}, m_item);
}

struct menu_item_visitor_click
{
    void operator()(button& item)
    {
      item.Click();
    }

    void operator()(menu_slider& item)
    {
    }
};

inline auto menu_item::Click() -> void
{
  std::visit(menu_item_visitor_click {}, m_item);
}
