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
  virtual ~menu();

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
