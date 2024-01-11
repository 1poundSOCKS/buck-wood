#pragma once

#include "particle_collision.h"

template <typename object_type, typename particle_type>
class particle_collision_results
{

public:

  using collision = std::tuple<object_type&, particle_type&>;
  using collision_collection = std::vector<collision>;

public:

  particle_collision_results() : 
    m_collisionTests { [this](auto& object, auto& particle) { m_results.emplace_back(object.Object(), particle); }}
  {
  }

  auto Fetch(std::ranges::input_range auto&& objects, std::ranges::input_range auto&& particles) -> void
  {
    m_collisionTests(objects, particles);
  }

  auto Fetch(dynamic_object<object_type>& object, std::ranges::input_range auto&& particles) -> void
  {
    m_collisionTests(object, particles);
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
      const auto& [object, particle] = result;
      binary_op(object, particle);
    }
  }

private:

  particle_collision<object_type, particle_type> m_collisionTests;
  collision_collection m_results;

};
