#include "pch.h"
#include "render_brushes.h"
#include "framework.h"

menu_brushes::menu_brushes()
{
  const auto& renderTarget = render_target::renderTarget().get();
  m_textBrush = screen_render_brush_white.CreateBrush(renderTarget);
  m_buttonBrush = screen_render_brush_grey.CreateBrush(renderTarget);
  m_buttonBorderBrush = screen_render_brush_white.CreateBrush(renderTarget);
  m_buttonHoverBrush = screen_render_brush_green.CreateBrush(renderTarget);
}

player_ship_brushes::player_ship_brushes() : filled_geometry_brushes { screen_render_brush_grey, screen_render_brush_white, 2 }
{
  const auto& renderTarget = render_target::renderTarget();
  m_thruster = screen_render_brush_red.CreateBrush(renderTarget.get());
}

[[nodiscard]] auto player_ship_brushes::Thruster() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_thruster;
}
