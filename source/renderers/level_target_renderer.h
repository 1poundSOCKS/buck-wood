#pragma once

#include "objects/level_target.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"

class level_target_renderer
{

public:

  auto Write(const dynamic_object<level_target>& targetObject) const -> void;

private:

  geometry_renderer m_deactivatedRenderer { screen_render_brush_black.CreateBrush(), screen_render_brush_red.CreateBrush(), 10 };
  geometry_renderer m_activatedRenderer { screen_render_brush_black.CreateBrush(), screen_render_brush_grey.CreateBrush(), 10 };

};

inline auto level_target_renderer::Write(const dynamic_object<level_target>& targetObject) const -> void
{
  targetObject->IsActivated() ? m_activatedRenderer.Write(targetObject.Geometry()) : m_deactivatedRenderer.Write(targetObject.Geometry());
}
