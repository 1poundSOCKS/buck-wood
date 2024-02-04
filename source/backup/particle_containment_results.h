#pragma once

#include "particle_containment.h"

template <typename particle_type>
class particle_containment_results
{

public:

  using containment_type = std::reference_wrapper<particle_type>;
  using containment_collection = std::vector<containment_type>;

public:

  particle_containment_results() : 
    m_containmentTests { [this](auto& particle) { m_results.emplace_back(particle); }}
  {
  }

  auto Fetch(ID2D1Geometry* containmentGeometry, std::ranges::input_range auto&& particles) -> void
  {
    m_containmentTests(containmentGeometry, particles);
  }

  [[nodiscard]] auto Get() const -> const containment_collection
  {
    return m_results;
  }

  auto Clear() -> void
  {
    m_results.clear();
  }

  [[nodiscard]] auto Count() const -> containment_collection::size_type
  {
    return m_results.size();
  }

  auto Process(auto&& unary_op) -> void
  {
    for( auto& particleWrapper : m_results )
    {
      unary_op(particleWrapper.get());
    }
  }

private:

  particle_containment<particle_type> m_containmentTests;
  containment_collection m_results;

};
