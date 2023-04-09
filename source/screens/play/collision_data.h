#pragma once

#include "framework.h"

class collision_data
{
public:

  collision_data()
  {
  }

  template <typename points_iterator_type>
  collision_data(points_iterator_type begin, points_iterator_type end) : m_closedObject(begin, end)
  {
  }

  collision_data(const game_closed_object& closedObject) : m_closedObject(closedObject)
  {
  }

  auto operator=(const collision_data& collisionData) -> const collision_data&
  {
    m_closedObject = collisionData.m_closedObject;
    return *this;
  }

  [[nodiscard]] auto PointInside(float x, float y) const -> bool
  {
    return ::PointInside(x, y, m_closedObject);
  }

private:

  game_closed_object m_closedObject;
};
