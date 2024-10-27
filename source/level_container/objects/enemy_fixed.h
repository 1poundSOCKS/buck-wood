#pragma once

class enemy_fixed
{

public:

  [[nodiscard]] auto operator()(POINT_2F position, float speed, float interval) -> POINT_2F;

};

inline auto enemy_fixed::operator()(POINT_2F position, float speed, float interval) -> POINT_2F
{
  return position;
}
