#pragma once

template <typename geometry_object_type, typename particle_object_type>
class particle_collision
{

public:

  particle_collision(auto&& callable) : m_callable { callable }
  {
  }

  auto operator()(dynamic_object<geometry_object_type>& geometryObject, particle_object_type& particleObject) -> void
  {
    auto position1 = geometryObject->Position();
    auto position2 = particleObject.Position();

    if( direct2d::GetDistanceBetweenPoints(position1, position2) < geometryObject.GeometryRadius() )
    {
      BOOL collision = FALSE;
      HRESULT hr = geometryObject.Geometry()->FillContainsPoint({particleObject.Position().x, particleObject.Position().y}, D2D1::Matrix3x2F::Identity(), &collision);

      if( SUCCEEDED(hr) && collision )
      {
        std::lock_guard<std::mutex> guard(m_mutex);
        m_callable(geometryObject, particleObject);
      }
    }
  }

  auto operator()(dynamic_object<geometry_object_type>& geometryObject, std::ranges::input_range auto && particleObjectCollection) -> void
  {
    std::for_each(std::execution::par_unseq, std::begin(particleObjectCollection), std::end(particleObjectCollection), [this,&geometryObject](auto& particleObject)
    {
      (*this)(geometryObject, particleObject);
    });
  }

  auto operator()(std::ranges::input_range auto&& geometryObjectCollection, std::ranges::input_range auto && particleObjectCollection) -> void
  {
    std::for_each(std::execution::par_unseq, std::begin(geometryObjectCollection), std::end(geometryObjectCollection), [this,&particleObjectCollection](auto& geometryObject)
    {
      for( auto& particleObject : particleObjectCollection )
      {
        (*this)(geometryObject, particleObject);
      }
    });
  }

private:

  std::function<void(dynamic_object<geometry_object_type>&, particle_object_type&)> m_callable;
  std::mutex m_mutex;

};
