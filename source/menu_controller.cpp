#include "pch.h"
#include "menu_controller.h"

// auto menu_controller::Open(const D2D1_RECT_F& area) -> void
// {
//   m_menus.emplace( menu { area } );
// }

auto menu_controller::Close() -> void
{
  if( m_menus.size() > 2 )
  {
    m_menus.pop();
  }
}

auto menu_controller::Update(const menu_control_data& controlData) -> void
{
  GetCurrent().Update(controlData);

  if( controlData.Back() )
  {
    Close();
  }
}
