#pragma once

#include "enemy_destination.h"

class enemy_area
{

public:

  enemy_area(std::ranges::input_range auto&& points);

  [[nodiscard]] auto operator()(POINT_2F position, float speed, float interval) -> POINT_2F;

private:

  using points_collection = std::vector<POINT_2F>;
  points_collection m_points;
  std::uniform_int_distribution<points_collection::size_type> m_pointIndex;
  enemy_destination m_destination;

};

inline enemy_area::enemy_area(std::ranges::input_range auto &&points) : 
  m_points { points }, 
  m_pointIndex { 0, m_points.size() - 1 }
{
}

inline auto enemy_area::operator()(POINT_2F position, float speed, float interval) -> POINT_2F
{
  return m_destination(position, speed, interval, [this]() -> POINT_2F
  {
    return m_points[m_pointIndex(pseudo_random_generator::get())];
  });
}
