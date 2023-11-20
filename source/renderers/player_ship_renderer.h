#pragma once

#include "render_types.h"
#include "player_ship.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"

class player_ship_renderer
{

public:

  using points_collection = std::vector<game_point>;

  player_ship_renderer();
  auto Write(const player_ship& playerShip) const -> void;

private:

  geometry_renderer m_renderer { screen_render_brush_dark_grey.CreateBrush(), screen_render_brush_white.CreateBrush(), 2 };
  path_geometry m_thrusterGeometry;
  geometry_renderer m_thrusterRenderer { screen_render_brush_red.CreateBrush() };

};
