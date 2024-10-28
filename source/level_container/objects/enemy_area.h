#pragma once

#include "cell_id.h"
#include "cell_size.h"

class enemy_area
{

public:

  enemy_area(std::ranges::input_range auto&& points);

  [[nodiscard]] auto operator()(POINT_2F position, float speed, float interval) -> POINT_2F;

private:

  [[nodiscard]] auto Move(POINT_2F position, float distance, float interval) -> std::pair<POINT_2F,float>;

  using points_collection = std::vector<POINT_2F>;
  points_collection m_points;
  std::uniform_int_distribution<points_collection::size_type> m_pointIndex;
  POINT_2F m_destination;

};

inline enemy_area::enemy_area(std::ranges::input_range auto &&points) : 
  m_points { points }, 
  m_pointIndex { 0, m_points.size() - 1 }, 
  m_destination { m_points[m_pointIndex(pseudo_random_generator::get())] }
{
}

inline auto enemy_area::operator()(POINT_2F position, float speed, float interval) -> POINT_2F
{
  auto newPosition = position;
  auto remainingInterval = interval;

  while( remainingInterval > 0.0f )
  {
    auto&& [endOfMovePosition, endOfMoveInterval] = Move(position, speed * remainingInterval, interval);
    newPosition = endOfMovePosition;
    remainingInterval = endOfMoveInterval;
  }

  return newPosition;
}

inline auto enemy_area::Move(POINT_2F position, float distance, float interval) -> std::pair<POINT_2F, float>
{
  auto distanceToDest = direct2d::GetDistanceBetweenPoints(position, m_destination);

  if( distance > distanceToDest && distanceToDest > 0.0f )
  {
    auto distanceLeft = distance - distanceToDest;
    auto intervalLeft = interval * distanceLeft / distance;
    return Move(std::exchange(m_destination, m_points[m_pointIndex(pseudo_random_generator::get())]), distanceLeft, intervalLeft);
  }
  else
  {
    auto direction = direct2d::GetAngleBetweenPoints(position, m_destination);
    return { direct2d::CalculatePosition(position, direction, distance), 0.0f };
  }
}
