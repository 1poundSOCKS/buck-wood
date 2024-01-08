#include "pch.h"
#include "menu.h"
#include "renderers.h"
#include "render_target_area.h"
#include "column_def.h"

menu::menu()
{
  auto renderTargetSize = render_target::get()->GetSize();
  m_rect = render_rect { 0, 0, renderTargetSize.width - 1, renderTargetSize.height - 1 };
}

menu::~menu()
{
  Unselect();
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
  render_target_area menuArea { m_rect, render_target_area::constraint_centred(0.5, std::min(m_items.size() * 0.15f, 0.8f)) };
  column_def columnDefs { menuArea.GetRect(), m_items.size() };
  auto columnDef = std::begin(columnDefs);
  
  for( auto& item : m_items )
  {
    const auto& columnRect = *columnDef;
    item.Resize(columnRect);
    ++columnDef;
  }
}

auto menu::SelectFirstItem() -> void
{
  Unselect();

  if( m_items.size() > 0 )
  {
    m_items.front().Select();
    m_items.front().SetHoverState(true);
  }
}

auto menu::Unselect() -> void
{
  for( auto& item : m_items )
  {
    item.Unselect();
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
    m_items[selectedItem].Select();
    m_items[selectedItem].SetHoverState(true);
  }
}

auto menu::SelectPreviousItem(item_selection_type currentItem) -> void
{
  auto selectedItem = GetSelectedItem();

  if( ValidItem(--selectedItem) )
  {
    Unselect();
    m_items[selectedItem].Select();
    m_items[selectedItem].SetHoverState(true);
  }
}

[[nodiscard]] auto menu::ValidItem(item_selection_type selectedItem) const -> bool
{
  return selectedItem >= 0 && selectedItem < m_items.size() ? true : false;
}
