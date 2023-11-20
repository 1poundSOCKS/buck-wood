#pragma once

#include "level_target.h"
#include "geometry_renderer.h"
#include "render_brush_collection.h"
#include "screen_render_brush_defs.h"

class level_target_renderer
{

public:

  auto Write(const level_target& levelTarget) const -> void;

private:

  geometry_renderer m_geometryRenderer;

  enum class brush_type { fill=0, activated,  not_activated };
  using brush_collection_type = render_brush_collection<brush_type>;

  brush_collection_type m_brushes
  {
    std::initializer_list<brush_collection_type::initializer_type>
    {
      brush_collection_type::initializer_type { brush_type::fill, screen_render_brush_dark_grey }, 
      brush_collection_type::initializer_type { brush_type::activated, screen_render_brush_red },
      brush_collection_type::initializer_type { brush_type::not_activated, screen_render_brush_green }
  }};

};

inline auto level_target_renderer::Write(const level_target& levelTarget) const -> void
{
  levelTarget.IsActivated() ? m_geometryRenderer.Write(levelTarget.Geometry(), m_brushes[brush_type::fill], m_brushes[brush_type::activated], 5) :
    m_geometryRenderer.Write(levelTarget.Geometry(), m_brushes[brush_type::fill], m_brushes[brush_type::not_activated], 5);
}
