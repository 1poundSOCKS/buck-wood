#pragma once

#include "framework.h"
#include "game_scale.h"

class level_asteroid
{

public:

  level_asteroid(float x, float y, float maxWidth, float maxHeight);

  [[nodiscard]] auto Scale() const -> game_scale
  {
    return { 1.0f, 1.0f };
  }

  [[nodiscard]] auto Angle() const -> float
  {
    return 0;
  }

  [[nodiscard]] auto Position() const -> const game_point&
  {
    return m_position;
  }

private:

  game_point m_position { 0, 0 };

};
