#pragma once

#include "framework.h"

class enemy_movement_random
{

public:

  // [[nodiscard]] auto operator()(POINT_2F position, std::optional<POINT_2F> destination, float interval) -> std::tuple<POINT_2F, std::optional<POINT_2F>>;
  // [[nodiscard]] auto operator()(/POINT_2F position, std::optional<POINT_2F> destination, float interval) -> std::optional<POINT_2F>;
  [[nodiscard]] auto operator()() -> POINT_2F;

private:

  inline static std::uniform_int_distribution<int> m_positionDist { -10, 10 };

};

inline [[nodiscard]] auto enemy_movement_random::operator()() -> POINT_2F
{
  // constexpr float speed = 500;

  // auto newDestination = destination ? destination : std::optional<POINT_2F>({static_cast<float>(m_positionDist(pseudo_random_generator::get())) * 100, static_cast<float>(m_positionDist(pseudo_random_generator::get())) * 100});
  // return destination ? destination : std::optional<POINT_2F>({static_cast<float>(m_positionDist(pseudo_random_generator::get())) * 100, static_cast<float>(m_positionDist(pseudo_random_generator::get())) * 100});
  // auto newPosition = newDestination ? direct2d::MoveTowards(position, *newDestination, speed * interval) : position;
  // newDestination = newDestination && direct2d::AreEqual(newPosition, *newDestination) ? std::nullopt : newDestination;

  // return { newPosition, newDestination };
  return { static_cast<float>(m_positionDist(pseudo_random_generator::get())) * 100, static_cast<float>(m_positionDist(pseudo_random_generator::get())) * 100 };
}
