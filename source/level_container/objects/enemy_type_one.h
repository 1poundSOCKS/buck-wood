#pragma once

#include "framework.h"
#include "base_object.h"

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

class enemy_type_one : public base_object
{

public:

  enemy_type_one(POINT_2F position);
  auto Update(float interval) -> void;

  constexpr [[nodiscard]] auto Health() const -> float;

private:

  constexpr static auto m_path { GetEnemyPath() };
  constexpr static float m_speed { 500 };
  int m_destinationIndex { 0 };

};

inline enemy_type_one::enemy_type_one(POINT_2F position) : base_object { position, { 0, 0 }, { 1, 1 }, 0 }
{
}

inline auto enemy_type_one::Update(float interval) -> void
{
  auto currentDestination = m_path[m_destinationIndex];
  auto distanceToDestination = direct2d::GetDistanceBetweenPoints(m_position, currentDestination);
  auto distanceToMove = m_speed * interval;

  if( distanceToDestination < distanceToMove )
  {
    m_position = currentDestination;
    m_destinationIndex = ++m_destinationIndex < m_path.size() ? m_destinationIndex : 0;
  }
  else
  {
    m_position = direct2d::MoveTowards(m_position, currentDestination, distanceToMove);
  }

  base_object::Update(interval);
}

constexpr [[nodiscard]] auto enemy_type_one::Health() const -> float
{
  return 1.0f;
}
