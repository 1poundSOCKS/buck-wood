#pragma once

#include "default_object_geometry.h"
#include "range_comparision_runner.h"

class default_object_geometry_collection
{

public:

  using value_type = std::vector<default_object_geometry>;
  using iterator = value_type::iterator;
  using const_iterator = value_type::const_iterator;


public:

  default_object_geometry_collection(size_t initialCapacity);

  auto Update(std::ranges::input_range auto& objects) -> void;

  [[nodiscard]] auto begin() noexcept -> iterator;
  [[nodiscard]] auto end() noexcept -> iterator;

  [[nodiscard]] auto begin() const noexcept -> const_iterator;
  [[nodiscard]] auto end() const noexcept -> const_iterator;

private:

  value_type m_geometries;

};

auto default_object_geometry_collection::Update(std::ranges::input_range auto &objects) -> void
{
  m_geometries.clear();

  for( auto& object : objects )
  {
    m_geometries.emplace_back(object);
  }
}

inline auto default_object_geometry_collection::begin() noexcept -> iterator
{
  return std::begin(m_geometries);
}

inline auto default_object_geometry_collection::end() noexcept -> iterator
{
  return std::end(m_geometries);
}

inline auto default_object_geometry_collection::begin() const noexcept -> const_iterator
{
  return std::begin(m_geometries);
}

inline auto default_object_geometry_collection::end() const noexcept -> const_iterator
{
  return std::end(m_geometries);
}
