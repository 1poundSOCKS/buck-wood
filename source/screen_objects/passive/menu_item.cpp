#include "pch.h"
#include "menu_item.h"

auto menu_item::HoverState() const -> bool
{
  struct menu_item_visitor_get_hover_state
  {
      void operator()(const button& item)
      {
        m_hoverState = item.HoverState();
      }

      void operator()(const setting_slider& item)
      {
        m_hoverState = item.HoverState();
      }

      bool m_hoverState { false };
  };

  menu_item_visitor_get_hover_state visitor {};
  std::visit(visitor, m_item);
  return visitor.m_hoverState;
}

auto menu_item::SetHoverState(bool value) -> void
{
  struct visitor
  {
    visitor(bool hoverState) : m_hoverState { hoverState }
    {
    }

    void operator()(button& item)
    {
      item.SetHoverState(m_hoverState);
    }

    void operator()(setting_slider& item)
    {
      item.SetHoverState(m_hoverState);
    }

    bool m_hoverState { false };
  };

  std::visit(visitor { value }, m_item);
}

auto menu_item::Click() -> void
{
  struct visitor
  {
      void operator()(button& item)
      {
        item.Click();
      }

      void operator()(setting_slider& item)
      {
      }
  };

  std::visit(visitor {}, m_item);
}

auto menu_item::Resize(const D2D1_RECT_F& rect) -> void
{
  struct visitor
  {
    visitor(const D2D1_RECT_F& rect) : m_rect { rect }
    {
    }

    void operator()(button& item)
    {
      item.Resize(m_rect);
    }

    void operator()(setting_slider& item)
    {
      item.Resize(m_rect);
    }

    D2D1_RECT_F m_rect;
  };

  std::visit(visitor { rect }, m_item);
}

auto menu_item::Update(const menu_control_data& controlData) -> void
{
  struct visitor
  {
    visitor(const menu_control_data& controlData) : m_controlData { controlData }
    {
    }

    void operator()(button& item)
    {
    }

    void operator()(setting_slider& item)
    {
    }

    const menu_control_data& m_controlData;
  };

  std::visit(visitor { controlData }, m_item);
}
