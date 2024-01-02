#pragma once

#include "game_point.h"

class hud_target
{

public:

  hud_target(game_point position, D2D1_RECT_F bounds) : m_position { position }, m_bounds { bounds }
  {
  }

  [[nodiscard]] auto Position() const -> game_point
  {
    return m_position;
  }

  [[nodiscard]] auto Bounds() const -> D2D1_RECT_F
  {
    return m_bounds;
  }

private:

  game_point m_position;
  D2D1_RECT_F m_bounds;

};
