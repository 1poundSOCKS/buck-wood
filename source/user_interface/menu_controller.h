#pragma once

#include "menu.h"
#include "menu_control_data.h"

class menu_controller
{

public:

  menu_controller();
  auto Open() -> void;
  auto Close() -> void;
  auto Update() -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;
  auto Render() const -> void;
  [[nodiscard]] auto GetCurrent() const-> const menu&;
  [[nodiscard]] auto GetCurrent() -> menu&;
  auto Clear() noexcept -> void;

private:

  using menu_collection = std::stack<menu>;
  menu_collection m_menus;

};

inline menu_controller::menu_controller()
{
  m_menus.emplace();
}

inline auto menu_controller::Open() -> void
{
  m_menus.emplace();
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

inline auto menu_controller::Clear() noexcept -> void
{
  while( !m_menus.empty() ) m_menus.pop();
  m_menus.emplace();
}
