#pragma once

#include "framework.h"

class enemy_movement_random
{

public:

  [[nodiscard]] auto Position(POINT_2F position, float interval) -> POINT_2F;

private:

  inline static std::uniform_int_distribution<int> m_positionDist { -10, 10 };
  std::optional<POINT_2F> m_destination;

};

inline [[nodiscard]] auto enemy_movement_random::Position(POINT_2F position, float interval) -> POINT_2F
{
  constexpr float speed = 500;

  m_destination = m_destination ? m_destination : std::optional<POINT_2F>({static_cast<float>(m_positionDist(pseudo_random_generator::get())) * 100, static_cast<float>(m_positionDist(pseudo_random_generator::get())) * 100});
  auto newPosition = m_destination ? direct2d::MoveTowards(position, *m_destination, speed * interval) : position;
  m_destination = m_destination && direct2d::AreEqual(newPosition, *m_destination) ? std::nullopt : m_destination;

  return newPosition;
}
