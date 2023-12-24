#pragma once

#include "framework.h"

class level_asteroid
{

public:

  level_asteroid(float x, float y, float maxWidth, float maxHeight);

  [[nodiscard]] auto Position() const -> const game_point&
  {
    return m_position;
  }

  [[nodiscard]] auto Angle() const -> float
  {
    return 0;
  }

private:

  game_point m_position { 0, 0 };

};
