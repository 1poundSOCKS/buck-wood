#pragma once

class player_destination
{

public:

  player_destination(D2D1_POINT_2F position) : m_position { position }
  {
  }

  [[nodiscard]] auto Position() const -> D2D1_POINT_2F
  {
    return m_position;
  }

private:

  D2D1_POINT_2F m_position;

};
