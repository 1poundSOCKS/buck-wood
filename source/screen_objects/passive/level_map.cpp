#include "pch.h"
#include "level_map.h"

level_map::level_map()
{
  const auto& renderTarget = framework::renderTarget();
  m_borderBrush = screen_render_brush_white.CreateBrush(renderTarget.get());
  m_playerBrush = screen_render_brush_yellow.CreateBrush(renderTarget.get());
  m_activatedTargetBrush = screen_render_brush_red.CreateBrush(renderTarget.get());
  m_deactivatedTargetBrush = screen_render_brush_green.CreateBrush(renderTarget.get());
}

auto level_map::SetRect(game_rect rect) -> void
{
  m_rect = rect;
}
