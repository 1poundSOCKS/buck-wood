#pragma once

#include "geometry_collision.h"

template <typename object_type_1, typename object_type_2>
class geometry_collision_results
{

public:

  using collision = std::tuple<object_type_1&, object_type_2&>;
  using collision_collection = std::vector<collision>;

public:

  geometry_collision_results() : 
    m_collisionTests { [this](auto& object1, auto& object2) { m_results.emplace_back(object1, object2); }}
  {
  }

  auto Fetch(std::ranges::input_range auto&& type1Objects, std::ranges::input_range auto&& type2Objects) -> void
  {
    m_collisionTests(type1Objects, type2Objects);
  }

  auto Fetch(dynamic_object<object_type_1>& type1Object, std::ranges::input_range auto&& type2Objects) -> void
  {
    m_collisionTests(type1Object, type2Objects);
  }

  [[nodiscard]] auto Get() const -> const collision_collection
  {
    return m_results;
  }

  auto Clear() -> void
  {
    m_results.clear();
  }

  [[nodiscard]] auto Count() const -> collision_collection::size_type
  {
    return m_results.size();
  }

  auto Process(auto&& binary_op) -> void
  {
    for( const auto& result : m_results )
    {
      const auto& [type1Object, type2Object] = result;
      binary_op(type1Object, type2Object);
    }
  }

private:

  geometry_collision<object_type_1, object_type_2> m_collisionTests;
  collision_collection m_results;

};
