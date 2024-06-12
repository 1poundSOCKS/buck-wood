#pragma once

#include "default_object_geometry.h"
#include "range_comparision_runner.h"

class default_object_geometry_collection
{

public:

  default_object_geometry_collection(size_t initialCapacity);

  auto Update(std::ranges::input_range auto& objects) -> void;
  // auto Enumerate(auto&& visitor) const -> void;
  auto Visit(auto&& visitor) -> void;

private:

  std::vector<default_object_geometry> m_geometries;

};

auto default_object_geometry_collection::Update(std::ranges::input_range auto &objects) -> void
{
  m_geometries.clear();

  for( auto& object : objects )
  {
    m_geometries.emplace_back(object);
  }
}

// auto default_object_geometry_collection::Enumerate(auto &&visitor) const -> void
// {
//   for( const auto& geometry : m_geometries )
//   {
//     visitor(geometry);
//   }
// }

auto default_object_geometry_collection::Visit(auto&& visitor) -> void
{
  visitor(m_geometries);
}
