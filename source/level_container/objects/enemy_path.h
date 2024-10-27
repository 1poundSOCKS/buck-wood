#pragma once

#include "framework.h"

class enemy_path
{

public:

  enemy_path(POINT_2F position, bool loop);
  enemy_path(std::ranges::input_range auto&& points, bool loop=true);

  [[nodiscard]] auto operator()(POINT_2F position, float speed, float interval) -> POINT_2F;

  auto AddPoint(POINT_2F point) -> void;

private:

  [[nodiscard]] auto Move(POINT_2F position, float distance, float interval) -> POINT_2F;

  using container_type = std::vector<POINT_2F>;
  container_type m_points;
  container_type::size_type m_destination;
  bool m_loop;

};

inline enemy_path::enemy_path(POINT_2F position, bool loop) : m_points { position }, m_loop { loop }
{
}

enemy_path::enemy_path(std::ranges::input_range auto &&points, bool loop) : m_destination{ 0 }, m_loop{ loop }
{
  std::ranges::copy(points, std::back_inserter(m_points));
}

inline auto enemy_path::operator()(POINT_2F position, float speed, float interval) -> POINT_2F
{
  return m_points.size() > 0 && interval > 0.0f ? Move(position, speed * interval, interval) : position;
}

inline auto enemy_path::AddPoint(POINT_2F point) -> void
{
  m_points.push_back(point);
}

inline auto enemy_path::Move(POINT_2F position, float distance, float interval) -> POINT_2F
{
  auto destination = m_points[m_destination];
  auto distanceToDest = direct2d::GetDistanceBetweenPoints(position, destination);

  if( distance > distanceToDest && distanceToDest > 0.0f )
  {
    auto distanceLeft = distance - distanceToDest;
    auto intervalLeft = interval * distanceLeft / distance;
    auto waypoint = m_points[m_destination];
    m_destination = ( ++m_destination < m_points.size() ) ? m_destination : 0;
    return Move(waypoint, distanceLeft, intervalLeft);
  }
  else
  {
    auto direction = direct2d::GetAngleBetweenPoints(position, destination);
    return direct2d::CalculatePosition(position, direction, distance);
  }
}
