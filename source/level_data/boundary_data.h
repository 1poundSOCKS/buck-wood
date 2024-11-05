#pragma once

#include "framework.h"
#include "geometry/image_data.h"
#include "level_data.h"
#include "geometry/point_data.h"

class boundary_data
{

public:

  static auto create(int cellWidth, int cellHeight) noexcept -> void;
  static auto destroy() noexcept -> void;
  static auto get(int levelIndex) -> const std::vector<POINT_2F>&;
  static auto getInnerWallCount(int levelIndex) -> int;
  static auto getInnerWall(int levelIndex, int innerWallIndex) -> const std::vector<POINT_2F>&;
  static [[nodiscard]] auto indexIsValid(int index) noexcept -> bool;

private:

  boundary_data(int cellWidth, int cellHeight);
  auto Add(int index, int cellWidth, int cellHeight) noexcept -> void;
  static [[nodiscard]] auto Bounds(std::ranges::input_range auto&& cellData) noexcept -> RECT_I;

  inline static boundary_data* m_instance { nullptr };
  std::map<int, std::vector<POINT_2F>> m_boundaryData;
  std::map<int, std::vector<std::vector<POINT_2F>>> m_innerWallData;

};

inline auto boundary_data::create(int cellWidth, int cellHeight) noexcept -> void
{
  destroy();
  m_instance = new boundary_data(cellWidth, cellHeight);
}

inline auto boundary_data::destroy() noexcept -> void
{
  delete m_instance;
  m_instance = nullptr;
}

inline auto boundary_data::get(int index) -> const std::vector<POINT_2F> &
{
  return m_instance->m_boundaryData[index];
}

inline auto boundary_data::getInnerWallCount(int levelIndex) -> int
{
  return static_cast<int>(m_instance->m_innerWallData[levelIndex].size());
}

inline auto boundary_data::getInnerWall(int levelIndex, int innerWallIndex) -> const std::vector<POINT_2F> &
{
  return m_instance->m_innerWallData[levelIndex][innerWallIndex];
}

inline auto boundary_data::indexIsValid(int index) noexcept -> bool
{
  return m_instance->m_boundaryData.contains(index);
}

inline boundary_data::boundary_data(int cellWidth, int cellHeight)
{
  Add(0, cellWidth, cellHeight);
  Add(1, cellWidth, cellHeight);
  Add(2, cellWidth, cellHeight);
}

auto boundary_data::Bounds(std::ranges::input_range auto &&cellData) noexcept -> RECT_I
{
  static_assert(std::is_same_v<std::ranges::range_value_t<decltype(cellData)>, std::tuple<int, int, level_data::cell_type>>);

  return std::accumulate(std::begin(cellData), std::end(cellData), RECT_I { 0, 0, 0, 0 }, [](RECT_I bounds, auto&& cell) -> RECT_I
  {
    auto&& [column, row, type] = cell;
    return { std::min(bounds.left, column), std::min(bounds.top, row), std::max(bounds.right, column), std::max(bounds.bottom, row) };
  });
}
