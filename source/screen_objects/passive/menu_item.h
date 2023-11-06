#pragma once

#include "button.h"
#include "slider.h"

class menu_item
{

public:

  using item_type = std::variant<button, slider>;

  menu_item(const button& item);
  menu_item(const slider& item);

  [[nodiscard]] auto Get() const -> const item_type&;

  [[nodiscard]] auto HoverState() const -> bool;
  auto SetHoverState(bool value) -> void;

  auto Click() -> void;

  auto Resize(const D2D1_RECT_F& rect) -> void;

private:

  item_type m_item;

};

inline menu_item::menu_item(const button& item) : m_item { item }
{
}

inline menu_item::menu_item(const slider& item) : m_item { item }
{
}

[[nodiscard]] inline auto menu_item::Get() const -> const item_type&
{
  return m_item;
}

inline auto menu_item::HoverState() const -> bool
{
  struct menu_item_visitor_get_hover_state
  {
      void operator()(const button& item)
      {
        m_hoverState = item.HoverState();
      }

      void operator()(const slider& item)
      {
      }

      bool m_hoverState { false };
  };

  menu_item_visitor_get_hover_state visitor {};
  std::visit(visitor, m_item);
  return visitor.m_hoverState;
}

inline auto menu_item::SetHoverState(bool value) -> void
{
  struct menu_item_visitor_set_hover_state
  {
    menu_item_visitor_set_hover_state(bool hoverState) : m_hoverState { hoverState }
    {
    }

    void operator()(button& item)
    {
      item.SetHoverState(m_hoverState);
    }

    void operator()(slider& item)
    {
    }

    bool m_hoverState { false };
  };

  std::visit(menu_item_visitor_set_hover_state { value }, m_item);
}

inline auto menu_item::Click() -> void
{
  struct menu_item_visitor_click
  {
      void operator()(button& item)
      {
        item.Click();
      }

      void operator()(slider& item)
      {
      }
  };

  std::visit(menu_item_visitor_click {}, m_item);
}

inline auto menu_item::Resize(const D2D1_RECT_F& rect) -> void
{
  struct menu_item_visitor_resize
  {
    menu_item_visitor_resize(const D2D1_RECT_F& rect) : m_rect { rect }
    {
    }

    void operator()(button& item)
    {
      item.Resize(m_rect);
    }

    void operator()(slider& item)
    {
      item.Resize(m_rect);
    }

    D2D1_RECT_F m_rect;
  };

  std::visit(menu_item_visitor_resize { rect }, m_item);
}
