#pragma once

class enemy_area
{

public:

  [[nodiscard]] auto operator()(POINT_2F position, float speed, float interval) -> POINT_2F;

};

inline auto enemy_area::operator()(POINT_2F position, float speed, float interval) -> POINT_2F
{
  return position;
}
