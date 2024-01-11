#pragma once

template <typename contained_particle_type>
class particle_containment
{

public:

  particle_containment(auto&& callable) : m_callable { callable }
  {
  }

  auto operator()(ID2D1Geometry* containmentGeometry, contained_particle_type& particle) -> void
  {
    BOOL contains { FALSE };
    D2D1_POINT_2F position { particle.Position().x, particle.Position().y };
    HRESULT hr = containmentGeometry->FillContainsPoint(position, NULL, &contains);

    if( SUCCEEDED(hr) && !contains )
    {
      m_callable(particle);
    }
  }

  auto operator()(ID2D1Geometry* containmentGeometry, std::ranges::input_range auto&& containedParticles) -> void
  {
    std::for_each(std::execution::par_unseq, std::begin(containedParticles), std::end(containedParticles), [this,containmentGeometry](auto& particle)
    {
      std::lock_guard<std::mutex> guard(m_mutex);
      (*this)(containmentGeometry, particle);
    });
  }

private:

  std::function<void(contained_particle_type&)> m_callable;
  std::mutex m_mutex;

};
