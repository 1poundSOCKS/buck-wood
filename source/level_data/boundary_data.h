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
  static auto get(int index) -> const std::vector<POINT_2F>&;
  static [[nodiscard]] auto indexIsValid(int index) noexcept -> bool;

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

inline auto boundary_data::get(int index) -> const std::vector<POINT_2F> &
{
  return m_instance->m_boundaryData[index];
}

inline auto boundary_data::indexIsValid(int index) noexcept -> bool
{
  return m_instance->m_levelData.contains(index);
}

inline boundary_data::boundary_data(int cellWidth, int cellHeight)
{
  Add(0, cellWidth, cellHeight);
  Add(1, cellWidth, cellHeight);
  Add(2, cellWidth, cellHeight);
}
