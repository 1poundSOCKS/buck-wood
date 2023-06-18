#include "pch.h"
#include "level_radar.h"
#include "screen_render_brush_defs.h"

level_radar::level_radar()
{
  const auto& renderTarget = framework::renderTarget();
  m_brush = screen_render_brush_green.CreateBrush(renderTarget.get());
}
