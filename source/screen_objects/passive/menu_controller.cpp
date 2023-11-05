#include "pch.h"
#include "menu_controller.h"

auto menu_controller::Open(const menu_def& def) -> void
{
  m_menus.emplace( def.CreateMenu() );
  m_menus.top().SelectFirstItem();
}

auto menu_controller::Close() -> void
{
  if( m_menus.size() > 2 )
  {
    m_menus.pop();
  }
}

auto menu_controller::Update(const menu_control_data& controlData) -> void
{
  if( controlData.Back() )
  {
    Close();
  }
  else
  {
    GetCurrent().Update(controlData);
  }
}
