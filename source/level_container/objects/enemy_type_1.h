#pragma once

#include "framework.h"
#include "base_object.h"
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

class enemy_type_1 : public base_object
{

public:

  enemy_type_1(POINT_2F position);
  auto Update(float interval) -> void;

  constexpr [[nodiscard]] auto Health() const -> float;

private:

  constexpr static auto m_path { GetEnemyPath() };
  constexpr static float m_speed { 500 };
  int m_destinationIndex { 0 };

};

inline enemy_type_1::enemy_type_1(POINT_2F position) : base_object { position, { 1, 1 }, 0 }
{
}

inline auto enemy_type_1::Update(float interval) -> void
{
  auto destination = object_destination { m_path[m_destinationIndex] };
  m_position = destination.UpdatePosition(m_position, m_speed, interval);

  m_destinationIndex += destination == m_position ? 1 : 0;
  m_destinationIndex = m_destinationIndex < m_path.size() ? m_destinationIndex : 0;

  base_object::Update(interval);
}

constexpr [[nodiscard]] auto enemy_type_1::Health() const -> float
{
  return 1.0f;
}
