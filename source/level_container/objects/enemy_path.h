#pragma once

#include "framework.h"

class enemy_path
{

public:

  enemy_path(std::ranges::input_range auto&& points);

  [[nodiscard]] auto operator()(POINT_2F position, float interval) -> POINT_2F;

private:

  [[nodiscard]] auto Move(POINT_2F position, float distance, float interval) -> POINT_2F;

  constexpr static float m_speed { 400.0f };
  using container_type = std::vector<POINT_2F>;
  container_type m_points;
  container_type::const_iterator m_destination;

};

enemy_path::enemy_path(std::ranges::input_range auto&& points)
{
  std::ranges::copy(points, std::back_inserter(m_points));
  m_destination = std::begin(m_points);
}

inline auto enemy_path::operator()(POINT_2F position, float interval) -> POINT_2F
{
  return interval > 0.0f ? Move(position, m_speed * interval, interval) : position;
}

inline auto enemy_path::Move(POINT_2F position, float distance, float interval) -> POINT_2F
{
  auto destination = *m_destination;
  auto distanceToDest = direct2d::GetDistanceBetweenPoints(position, destination);

  if( distance > distanceToDest )
  {
    auto distanceLeft = distance - distanceToDest;
    auto intervalLeft = interval * distanceLeft / distance;
    auto waypoint = *m_destination;
    auto nextDestination = std::next(m_destination);
    m_destination = ( nextDestination == std::end(m_points) ) ? std::begin(m_points) : nextDestination;
    return Move(waypoint, distanceLeft, intervalLeft);
  }
  else
  {
    auto direction = direct2d::GetAngleBetweenPoints(position, destination);
    return direct2d::CalculatePosition(position, direction, distance);
  }
}
