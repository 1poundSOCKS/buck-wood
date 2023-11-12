#pragma once

#include "menu_item.h"
#include "menu_control_data.h"
#include "button.h"

class menu
{
public:

  using item_collection = std::vector<menu_item>;
  using item_selection_type = int;

  static const int invalid_item { -1 };

  menu(const D2D1_RECT_F& rect);

  auto AddItem(menu_item&& item, bool resize) -> void;
  auto SelectFirstItem() -> void;
  auto Unselect() -> void;
  auto Update(const menu_control_data& controlData) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  [[nodiscard]] auto GetSelectedItem() const -> item_selection_type;
  auto SelectNextItem(item_selection_type currentItem) -> void;
  auto SelectPreviousItem(item_selection_type currentItem) -> void;
  [[nodiscard]] auto ValidItem(item_selection_type currentItem) const -> bool;
  auto ResizeItems() -> void;
  
  D2D1_RECT_F m_rect;
  item_collection m_items;

};

inline auto menu::SelectFirstItem() -> void
{
  Unselect();

  if( m_items.size() > 0 )
  {
    m_items.front().SetHoverState(true);
  }
}

inline auto menu::Unselect() -> void
{
  for( auto& item : m_items )
  {
    item.SetHoverState(false);
  }
}

[[nodiscard]] inline auto menu::GetSelectedItem() const -> item_selection_type
{
  auto selectedItem = invalid_item;

  for( auto currentItem = 0; selectedItem == - 1 && currentItem < m_items.size(); ++currentItem )
  {
    if( m_items[currentItem].HoverState() )
    {
      selectedItem = currentItem;
    }
  }

  return selectedItem;
}

inline auto menu::SelectNextItem(item_selection_type currentItem) -> void
{
  auto selectedItem = GetSelectedItem();

  if( ValidItem(++selectedItem) )
  {
    Unselect();
    m_items[selectedItem].SetHoverState(true);
  }
}

inline auto menu::SelectPreviousItem(item_selection_type currentItem) -> void
{
  auto selectedItem = GetSelectedItem();

  if( ValidItem(--selectedItem) )
  {
    Unselect();
    m_items[selectedItem].SetHoverState(true);
  }
}

[[nodiscard]] inline auto menu::ValidItem(item_selection_type selectedItem) const -> bool
{
  return selectedItem >= 0 && selectedItem < m_items.size() ? true : false;
}
