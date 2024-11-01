#pragma once

#include "geometry/image_data.h"

class boundary_data
{

public:

  static auto create() noexcept -> void;
  static auto destroy() noexcept -> void;
  static auto get(int index) -> const std::vector<image_data::const_iterator::value_type>&;
  static [[nodiscard]] auto indexIsValid(int index) noexcept -> bool;

private:

  boundary_data();
  auto Add(int index, auto&& data) noexcept -> void;

  inline static boundary_data* m_instance { nullptr };
  std::map<int, std::vector<image_data::const_iterator::value_type>> m_levelData;
  std::map<int, std::vector<POINT_2F>> m_boundaryData;

};

inline auto boundary_data::create() noexcept -> void
{
  destroy();
  m_instance = new boundary_data();
}

inline auto boundary_data::destroy() noexcept -> void
{
  delete m_instance;
  m_instance = nullptr;
}

inline auto boundary_data::get(int index) -> const std::vector<image_data::const_iterator::value_type> &
{
  return m_instance->m_levelData[index];
}

inline auto boundary_data::indexIsValid(int index) noexcept -> bool
{
  return m_instance->m_levelData.contains(index);
}

inline boundary_data::boundary_data()
{
  Add(0, level_data::levelData_0);
  Add(1, level_data::levelData_1);
  Add(2, level_data::levelData_2);
}

auto boundary_data::Add(int index, auto &&data) noexcept -> void
{
  image_data imageData { data };
  std::pair<int, std::vector<image_data::const_iterator::value_type>> levelData;
  levelData.first = index;
  std::copy(std::begin(imageData), std::end(imageData), std::back_inserter(levelData.second));
  m_levelData.insert(levelData);
}
