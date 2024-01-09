#pragma once

#include "geometry_containment.h"

template <typename object_type>
class geometry_containment_results
{

public:

  using collision_collection = std::vector<std::reference_wrapper<object_type>>;

public:

  geometry_containment_results() : 
    m_containmentTests { [this](auto& object)
    {
      std::lock_guard<std::mutex> guard(m_mutex);
      m_results.emplace_back(object.Object());
    }}
  {
  }

  auto Fetch(ID2D1Geometry* containmentGeometry, auto& object) -> void
  {
    m_containmentTests(containmentGeometry, object);
  }

  auto Fetch(ID2D1Geometry* containmentGeometry, std::ranges::input_range auto&& objects) -> void
  {
    m_containmentTests(containmentGeometry, objects);
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

  auto Process(auto&& unary_op) -> void
  {
    for( const auto& result : m_results )
    {
      unary_op(result.get());
    }
  }

private:

  geometry_containment<object_type> m_containmentTests;
  collision_collection m_results;
  std::mutex m_mutex;

};
