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

  menu();
  virtual ~menu();

  template <typename item_type, class... Args> auto AddItem(Args&&... args) -> void;
  auto ResizeItems() -> void;
  auto SelectFirstItem() -> void;
  auto Unselect() -> void;
  auto Update(const menu_control_data& controlData) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:

  [[nodiscard]] auto GetSelectedItem() const -> item_selection_type;
  auto SelectNextItem(item_selection_type currentItem) -> void;
  auto SelectPreviousItem(item_selection_type currentItem) -> void;
  [[nodiscard]] auto ValidItem(item_selection_type currentItem) const -> bool;
  
  D2D1_RECT_F m_rect;
  item_collection m_items;

};

template <typename item_type, class... Args> auto menu::AddItem(Args&&... args) -> void
{
  m_items.emplace_back(std::in_place_type_t<item_type>(), std::forward<Args>(args)...);
}
