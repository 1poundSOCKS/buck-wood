#pragma once

#include "base_object.h"

class static_asteroid : public base_object
{

  public:
    static_asteroid(POINT_2F position, SCALE_2F scale, float angle) noexcept;

};

inline static_asteroid::static_asteroid(POINT_2F position, SCALE_2F scale, float angle) noexcept
 : base_object(position, scale, angle)
{

}
