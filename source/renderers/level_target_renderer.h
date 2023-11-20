#pragma once

#include "level_target.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"

class level_target_renderer
{

public:

  auto Write(const level_target& levelTarget) const -> void;

private:

  geometry_renderer m_deactivatedRenderer { screen_render_brush_dark_grey.CreateBrush(), screen_render_brush_red.CreateBrush(), 10 };
  geometry_renderer m_activatedRenderer { screen_render_brush_dark_grey.CreateBrush() };

};

inline auto level_target_renderer::Write(const level_target& levelTarget) const -> void
{
  levelTarget.IsActivated() ? m_activatedRenderer.Write(levelTarget.Geometry()) : m_deactivatedRenderer.Write(levelTarget.Geometry());
}
