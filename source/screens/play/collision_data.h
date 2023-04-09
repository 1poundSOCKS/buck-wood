#pragma once

#include "framework.h"

class collision_data
{
public:

  enum type { undefined, point, closed_object };

  collision_data()
  {
  }

  collision_data(float x, float y) : m_type(point), m_x(x), m_y(y)
  {
  }

  template <typename points_iterator_type>
  collision_data(points_iterator_type begin, points_iterator_type end) : m_type(closed_object), m_closedObject(begin, end)
  {
  }

  collision_data(const game_closed_object& closedObject) : m_type(closed_object), m_closedObject(closedObject)
  {
  }

  auto operator=(const collision_data& collisionData) -> const collision_data&
  {
    m_type = collisionData.m_type;
    m_x = collisionData.m_x;
    m_y = collisionData.m_y;
    m_closedObject = collisionData.m_closedObject;
    return *this;
  }

  [[nodiscard]] auto HasCollidedWith(const collision_data& collisionData) const -> bool
  {
    if( m_type == point )
    {
      switch( collisionData.m_type )
      {
        case point:
          return m_x == collisionData.m_x && m_y == collisionData.m_y;
        case closed_object:
          return ::PointInside(m_x, m_y, collisionData.m_closedObject);
        default:
          return false;
      }
    }
    else if( m_type == closed_object )
    {
      switch( collisionData.m_type )
      {
        case point:
          return ::PointInside(collisionData.m_x, collisionData.m_y, m_closedObject);
        case closed_object:
          return false; // TODO
        default:
          return false;
      }
    }
    else
      return false;
  }

  [[nodiscard]] auto PointInside(float x, float y) const -> bool
  {
    return ::PointInside(x, y, m_closedObject);
  }

private:

  type m_type = undefined;
  float m_x = 0;
  float m_y = 0;
  game_closed_object m_closedObject;
};
