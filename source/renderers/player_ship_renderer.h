#pragma once

#include "render_types.h"
#include "player_ship.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"

class player_ship_renderer
{

public:

  using points_collection = std::vector<game_point>;

  auto Write(const player_ship& playerShip) const -> void;

private:

  geometry_renderer m_renderer { screen_render_brush_white.CreateBrush(), 4 };

  inline static constexpr float m_thrusterWidth { 5 };
  inline static constexpr float m_thrusterTop { 14 };
  inline static constexpr float m_thrusterBottom { 20 };

  path_geometry m_thrusterGeometry { d2d_factory::get_raw(), 
    std::array { game_point { m_thrusterWidth, m_thrusterTop }, game_point { -m_thrusterWidth, m_thrusterTop }, 
    game_point { -m_thrusterWidth, m_thrusterBottom }, game_point { m_thrusterWidth, m_thrusterBottom } } };

  geometry_renderer m_thrusterRenderer { screen_render_brush_red.CreateBrush() };

};
