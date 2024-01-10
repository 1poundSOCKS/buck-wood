#pragma once

#include "framework.h"
#include "game_scale.h"

class level_asteroid
{

public:

  level_asteroid(D2D1_POINT_2F position, float maxWidth, float maxHeight);

  [[nodiscard]] auto Scale() const -> game_scale
  {
    return { 1.0f, 1.0f };
  }

  [[nodiscard]] auto Angle() const -> float
  {
    return 0;
  }

  [[nodiscard]] auto Position() const -> D2D1_POINT_2F
  {
    return m_position;
  }

private:

  D2D1_POINT_2F m_position;

};
