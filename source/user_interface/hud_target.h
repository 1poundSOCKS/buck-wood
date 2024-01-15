#pragma once

class hud_target
{

public:

  hud_target(D2D1_RECT_F bounds) : m_bounds { bounds }
  {
  }

  [[nodiscard]] auto Bounds() const -> D2D1_RECT_F
  {
    return m_bounds;
  }

private:

  D2D1_RECT_F m_bounds;

};
