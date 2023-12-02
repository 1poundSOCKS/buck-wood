#pragma once

#include "framework.h"
#include "geometry.h"
#include "color_scale_brushes.h"

class level_radar
{
public:

  level_radar(const game_point& position) : m_position { position }
  {
  }

  [[nodiscard]] auto AngleTo(const game_point& target) const -> float
  {
    return m_position.AngleTo(target);
  }

  [[nodiscard]] auto DistanceTo(const game_point& target) const -> float
  {
    return m_position.DistanceTo(target);
  }

private:

  game_point m_position;
  
};
