#include "pch.h"
#include "menu.h"
#include "renderers.h"
#include "column_def.h"

menu::menu(const D2D1_RECT_F& rect) : m_rect { rect }
{
}

auto menu::AddItem(menu_item&& item, bool resize) -> void
{
  m_items.emplace_back(item);

  if( resize )
  {
    ResizeItems();
  }
}

auto menu::Update(const menu_control_data& controlData) -> void
{
  auto selectedItem = GetSelectedItem();

  if( ValidItem(selectedItem) )
  {
    if( controlData.Down() )
    {
      SelectNextItem(selectedItem);
    }
    else if( controlData.Up() )
    {
      SelectPreviousItem(selectedItem);
    }
    else if( controlData.Select() )
    {
      m_items[selectedItem].Click();
    }
    else
    {
      m_items[selectedItem].Update(controlData);
    }
  }
}

auto menu::Render(D2D1_RECT_F viewRect) const -> void
{
  renderer::render_all(m_items);
}

auto menu::ResizeItems() -> void
{
  column_def columnDefs { m_rect, m_items.size() };

  auto columnDef = std::begin(columnDefs);
  
  for( auto& buttonObject : m_items )
  {
    const auto& columnRect = *columnDef;
    buttonObject.Resize(columnRect);
    ++columnDef;
  }
}

auto menu::SelectFirstItem() -> void
{
  Unselect();

  if( m_items.size() > 0 )
  {
    m_items.front().SetHoverState(true);
  }
}

auto menu::Unselect() -> void
{
  for( auto& item : m_items )
  {
    item.SetHoverState(false);
  }
}

[[nodiscard]] auto menu::GetSelectedItem() const -> item_selection_type
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

auto menu::SelectNextItem(item_selection_type currentItem) -> void
{
  auto selectedItem = GetSelectedItem();

  if( ValidItem(++selectedItem) )
  {
    Unselect();
    m_items[selectedItem].SetHoverState(true);
  }
}

auto menu::SelectPreviousItem(item_selection_type currentItem) -> void
{
  auto selectedItem = GetSelectedItem();

  if( ValidItem(--selectedItem) )
  {
    Unselect();
    m_items[selectedItem].SetHoverState(true);
  }
}

[[nodiscard]] auto menu::ValidItem(item_selection_type selectedItem) const -> bool
{
  return selectedItem >= 0 && selectedItem < m_items.size() ? true : false;
}
