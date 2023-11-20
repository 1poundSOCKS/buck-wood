#pragma once

#include "menu.h"
#include "menu_control_data.h"

class menu_controller
{

public:

  menu_controller();
  auto Open(auto && menuObject) -> void;
  auto Close() -> void;
  auto Update(const menu_control_data& controlData) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;
  [[nodiscard]] auto GetCurrent() const-> const menu&;
  [[nodiscard]] auto GetCurrent() -> menu&;

private:

  using menu_collection = std::stack<menu>;
  menu_collection m_menus;

};

inline menu_controller::menu_controller()
{
  m_menus.emplace( menu { { 0, 0, 0, 0 } } );
}

auto menu_controller::Open(auto && menuObject) -> void
{
  m_menus.emplace( menuObject );
}

inline auto menu_controller::Render(D2D1_RECT_F viewRect) const -> void
{
  GetCurrent().Render(viewRect);
}

[[nodiscard]] inline auto menu_controller::GetCurrent() const -> const menu&
{
  return m_menus.top();
}

[[nodiscard]] inline auto menu_controller::GetCurrent() -> menu&
{
  return m_menus.top();
}
