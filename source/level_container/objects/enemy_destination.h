#pragma once

#include "framework.h"

class enemy_destination
{

public:

  [[nodiscard]] auto operator()(POINT_2F position, float speed, float interval, auto &&getNextDestination) -> POINT_2F;

private:

  static [[nodiscard]] auto Move(POINT_2F position, POINT_2F destination, float distance) -> std::pair<POINT_2F,float>;

private:

  std::optional<POINT_2F> m_destination;

};

inline auto enemy_destination::operator()(POINT_2F position, float speed, float interval, auto &&getNextDestination) -> POINT_2F
{
  m_destination = m_destination ? m_destination : getNextDestination();
  auto distance = speed * interval;

  while( IsNotEqual(position, *m_destination) && distance > 0.0f )
  {
    auto&& [movePosition, remainingDistance] = Move(position, *m_destination, distance);
    m_destination = IsEqual(movePosition, position) ? getNextDestination() : m_destination;
    position = movePosition;
    distance = remainingDistance;
  }

  m_destination = IsEqual(position, *m_destination) ? getNextDestination() : m_destination;

  return position;
}

inline auto enemy_destination::Move(POINT_2F position, POINT_2F destination, float distance) -> std::pair<POINT_2F, float>
{
  auto distanceToDest = direct2d::GetDistanceBetweenPoints(position, destination);

  if( distanceToDest == 0.0f )
  {
    return { destination, 0.0f };
  }
  else if( distance > distanceToDest )
  {
    return { destination, distance - distanceToDest };
  }
  else
  {
    auto direction = direct2d::GetAngleBetweenPoints(position, destination);
    return { direct2d::CalculatePosition(position, direction, distance), 0.0f };
  }
}
