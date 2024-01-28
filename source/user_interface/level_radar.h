#pragma once

class level_radar
{
public:

  level_radar(D2D1_POINT_2F position) : m_position { position }
  {
  }

  [[nodiscard]] auto AngleTo(D2D1_POINT_2F target) const -> float
  {
    return direct2d::GetAngleBetweenPoints(m_position, target);
  }

  [[nodiscard]] auto DistanceTo(D2D1_POINT_2F target) const -> float
  {
    return direct2d::GetDistanceBetweenPoints(m_position, target);
  }

private:

  D2D1_POINT_2F m_position;
  
};
