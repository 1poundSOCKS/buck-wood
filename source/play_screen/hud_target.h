#pragma once

#include "game_point.h"

class hud_target
{

public:

  hud_target(game_point position, game_scale scale) : m_position { position }, m_scale { scale }
  {
  }

  [[nodiscard]] auto Position() const -> game_point
  {
    return m_position;
  }

  [[nodiscard]] auto Scale() const -> game_scale
  {
    return m_scale;
  }

private:

  game_point m_position;
  game_scale m_scale;

};
