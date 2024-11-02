#pragma once

#include "framework.h"
#include "geometry/image_data.h"
#include "level_data.h"

class boundary_data
{

public:

  static auto create(int cellWidth, int cellHeight) noexcept -> void;
  static auto destroy() noexcept -> void;
  // static auto get(int index) -> const std::vector<image_data::const_iterator::value_type>&;
  static auto getBoundary(int index) -> const std::vector<POINT_2F>&;
  static [[nodiscard]] auto indexIsValid(int index) noexcept -> bool;

  static [[nodiscard]] auto begin() noexcept -> std::map<int, std::vector<POINT_2F>>::const_iterator;
  static [[nodiscard]] auto end() noexcept -> std::map<int, std::vector<POINT_2F>>::const_iterator;

private:

  boundary_data(int cellWidth, int cellHeight);
  auto Add(int index, int cellWidth, int cellHeight) noexcept -> void;

  inline static boundary_data* m_instance { nullptr };
  std::map<int, std::vector<std::tuple<int, int, level_data::cell_type>>> m_levelData;
  std::map<int, std::vector<POINT_2F>> m_boundaryData;

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

// inline auto boundary_data::get(int index) -> const std::vector<image_data::const_iterator::value_type> &
// {
//   return m_instance->m_levelData[index];
// }

inline auto boundary_data::getBoundary(int index) -> const std::vector<POINT_2F> &
{
  return m_instance->m_boundaryData[index];
}

inline auto boundary_data::indexIsValid(int index) noexcept -> bool
{
  return m_instance->m_levelData.contains(index);
}

inline auto boundary_data::begin() noexcept -> std::map<int, std::vector<POINT_2F>>::const_iterator
{
  return std::begin(m_instance->m_boundaryData);
}

inline auto boundary_data::end() noexcept -> std::map<int, std::vector<POINT_2F>>::const_iterator
{
  return std::end(m_instance->m_boundaryData);
}

inline boundary_data::boundary_data(int cellWidth, int cellHeight)
{
  Add(0, cellWidth, cellHeight);
  Add(1, cellWidth, cellHeight);
  Add(2, cellWidth, cellHeight);
}
