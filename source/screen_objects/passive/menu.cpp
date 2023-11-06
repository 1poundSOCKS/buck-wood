#include "pch.h"
#include "menu.h"
#include "renderers.h"

menu::menu()
{
}

menu::menu(const menu& menuToCopy)
{
  m_items = menuToCopy.m_items;
}

auto menu::AddItem(menu_item&& item, bool resize) -> void
{
  m_items.emplace_back(item);
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
  }
}

auto menu::Render(D2D1_RECT_F viewRect) const -> void
{
  renderer::render_all(m_items);
}
