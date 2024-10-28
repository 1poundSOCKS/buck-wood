#pragma once

#include "cell_id.h"
#include "cell_size.h"

class enemy_area
{

public:

  enemy_area(std::ranges::input_range auto&& points);

  [[nodiscard]] auto operator()(POINT_2F position, float speed, float interval) -> POINT_2F;

private:

  [[nodiscard]] auto Move(POINT_2F position, float distance, float interval, auto&& getNextDestination) -> POINT_2F;
  static [[nodiscard]] auto Move(POINT_2F position, POINT_2F destination, float distance, float interval) -> std::pair<POINT_2F,float>;

  using points_collection = std::vector<POINT_2F>;
  points_collection m_points;
  std::uniform_int_distribution<points_collection::size_type> m_pointIndex;
  std::optional<POINT_2F> m_destination;

};

inline enemy_area::enemy_area(std::ranges::input_range auto &&points) : 
  m_points { points }, 
  m_pointIndex { 0, m_points.size() - 1 }
{
}

inline auto enemy_area::operator()(POINT_2F position, float speed, float interval) -> POINT_2F
{
  return Move(position, speed, interval, [this](POINT_2F position) -> POINT_2F
  {
    auto destination = m_points[m_pointIndex(pseudo_random_generator::get())];
    while( IsEqual(position, destination) ) { destination = m_points[m_pointIndex(pseudo_random_generator::get())]; }
    return destination;
  });
}

inline auto enemy_area::Move(POINT_2F position, float speed, float interval, auto &&getNextDestination) -> POINT_2F
{
  m_destination = m_destination ? m_destination : getNextDestination(position);
  auto distance = speed * interval;

  while( distance > 0.0f )
  {
    auto&& [movePosition, remainingDistance] = Move(position, *m_destination, distance, interval);
    m_destination = IsEqual(movePosition, position) ? getNextDestination(position) : m_destination;
    position = movePosition;
    distance = remainingDistance;
  }

  return position;
}

inline auto enemy_area::Move(POINT_2F position, POINT_2F destination, float distance, float interval) -> std::pair<POINT_2F, float>
{
  auto distanceToDest = direct2d::GetDistanceBetweenPoints(position, destination);

  if( distance > distanceToDest )
  {
    return { destination, distance - distanceToDest };
  }
  else
  {
    auto direction = direct2d::GetAngleBetweenPoints(position, destination);
    return { direct2d::CalculatePosition(position, direction, distance), 0.0f };
  }
}
