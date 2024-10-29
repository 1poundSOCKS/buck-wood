#pragma once

#include "enemy_destination.h"

class enemy_path
{

public:

  enemy_path(POINT_2F position);
  enemy_path(std::ranges::input_range auto&& points);

  [[nodiscard]] auto operator()(POINT_2F position, float speed, float interval) -> POINT_2F;

  auto AddPoint(POINT_2F point) -> void;

private:

  using container_type = std::vector<POINT_2F>;

  [[nodiscard]] auto GetCurrentIndexAndMoveNext() noexcept -> container_type::size_type;

private:

  container_type m_points;
  container_type::size_type m_pointsIndex;
  enemy_destination m_destination;

};

inline enemy_path::enemy_path(POINT_2F position) : m_points { position }, m_pointsIndex { 0 }
{
}

enemy_path::enemy_path(std::ranges::input_range auto &&points) : m_pointsIndex { 0 }
{
  std::ranges::copy(points, std::back_inserter(m_points));
}

inline auto enemy_path::operator()(POINT_2F position, float speed, float interval) -> POINT_2F
{
  return m_destination(position, speed, interval, [this]() -> POINT_2F
  {
    return m_points[GetCurrentIndexAndMoveNext()];
  });
}

inline auto enemy_path::AddPoint(POINT_2F point) -> void
{
  m_points.push_back(point);
}

inline auto enemy_path::GetCurrentIndexAndMoveNext() noexcept -> container_type::size_type
{
  auto nextIndex = m_pointsIndex + 1 < m_points.size() ? m_pointsIndex + 1 : 0;
  return std::exchange(m_pointsIndex, nextIndex);
}
