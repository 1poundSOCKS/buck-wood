#pragma once

template <typename contained_particle_type>
class particle_containment
{

public:

  // particle_containment(auto&& callable) : m_callable { callable }
  particle_containment()
  {
  }

  auto operator()(ID2D1Geometry* containmentGeometry, contained_particle_type& particle, auto&& callable) -> void
  {
    BOOL contains { FALSE };
    D2D1_POINT_2F position { particle.Position().x, particle.Position().y };
    HRESULT hr = containmentGeometry->FillContainsPoint(position, NULL, &contains);

    if( SUCCEEDED(hr) && !contains )
    {
      callable(particle);
    }
  }

  auto operator()(ID2D1Geometry* containmentGeometry, std::ranges::input_range auto&& containedParticles, auto&& callable) -> void
  {
    std::for_each(std::execution::par_unseq, std::begin(containedParticles), std::end(containedParticles), [this,containmentGeometry,&callable](auto& particle)
    {
      if( !particle.Destroyed() )
      {
        std::lock_guard<std::mutex> guard(m_mutex);
        (*this)(containmentGeometry, particle, callable);
      }
    });
  }

private:

  // std::function<void(contained_particle_type&)> m_callable;
  std::mutex m_mutex;

};
