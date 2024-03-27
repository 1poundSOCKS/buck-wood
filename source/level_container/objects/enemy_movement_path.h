#pragma once

#include "framework.h"
#include "object_destination.h"

constexpr auto GetEnemyPath()
{
  constexpr float distance = 1000;

  return std::array {
    POINT_2F { -distance, -distance },
    POINT_2F { -distance, distance },
    POINT_2F { distance, distance },
    POINT_2F { distance, -distance }
  };
}

class enemy_movement_path
{

public:

  [[nodiscard]] auto operator()(POINT_2F position, float interval) -> POINT_2F;

private:

  constexpr static auto m_path { GetEnemyPath() };
  int m_destinationIndex { 0 };

};

inline [[nodiscard]] auto enemy_movement_path::operator()(POINT_2F position, float interval) -> POINT_2F
{
  constexpr static float speed { 500 };

  auto destination = object_destination { m_path[m_destinationIndex] };
  POINT_2F newPosition = destination.UpdatePosition(position, speed, interval);

  m_destinationIndex += destination == newPosition ? 1 : 0;
  m_destinationIndex = m_destinationIndex < m_path.size() ? m_destinationIndex : 0;

  return newPosition;
}
