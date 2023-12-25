#pragma once

#include "game_point.h"

class hud_target
{

public:

  hud_target(const game_point& position) : m_position { position }
  {
  }

  [[nodiscard]] auto Position() const -> game_point
  {
    return m_position;
  }

private:

  game_point m_position;

};
