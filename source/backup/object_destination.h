#pragma once

#include "framework.h"

class object_destination
{

public:

  object_destination(POINT_2F value) noexcept : m_value { value }
  {
  }

  [[nodiscard]] auto UpdatePosition(POINT_2F position, float speed, float interval) const noexcept -> POINT_2F
  {
    auto distanceToDestination = direct2d::GetDistanceBetweenPoints(position, m_value);
    auto distanceToMove = speed * interval;
    return distanceToDestination < distanceToMove ? m_value : direct2d::MoveTowards(position, m_value, distanceToMove);
  }

  [[nodiscard]] auto operator==(POINT_2F position) const noexcept -> bool
  {
    return direct2d::AreEqual(position, m_value);
  }

private:

  POINT_2F m_value;

};
