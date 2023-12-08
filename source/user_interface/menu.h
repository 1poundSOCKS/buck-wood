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

  template <class... Args> auto AddButton(Args&&... args) -> void;
  template <class... Args> auto AddEffectsVolumeSlider(Args&&... args) -> void;
  template <class... Args> auto AddMusicVolumeSlider(Args&&... args) -> void;

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

template <class... Args> auto menu::AddButton(Args&&... args) -> void
{
  m_items.emplace_back(std::in_place_type<button>, std::forward<Args>(args)...);
}

template <class... Args> auto menu::AddEffectsVolumeSlider(Args&&... args) -> void
{
  m_items.emplace_back(std::in_place_type<effects_volume_slider>, std::forward<Args>(args)...);
}

template <class... Args> auto menu::AddMusicVolumeSlider(Args&&... args) -> void
{
  m_items.emplace_back(std::in_place_type<music_volume_slider>, std::forward<Args>(args)...);
}
