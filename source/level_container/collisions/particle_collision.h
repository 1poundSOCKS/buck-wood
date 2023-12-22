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
    BOOL collision = FALSE;
    HRESULT hr = geometryObject.Geometry().Get()->FillContainsPoint({particleObject.Position().x, particleObject.Position().y}, D2D1::Matrix3x2F::Identity(), &collision);

    if( SUCCEEDED(hr) && collision )
    {
      m_callable(geometryObject, particleObject);
    }
  }

  auto operator()(dynamic_object<geometry_object_type>& geometryObject, std::ranges::input_range auto && particleObjectCollection) -> void
  {
    for( auto& particleObject : particleObjectCollection )
    {
      (*this)(geometryObject, particleObject);
    }
  }

  auto operator()(dynamic_object_collection<geometry_object_type>& geometryObjectCollection, std::ranges::input_range auto && particleObjectCollection) -> void
  {
    for( auto& geometryObject : geometryObjectCollection )
    {
      for( auto& particleObject : particleObjectCollection )
      {
        (*this)(geometryObject, particleObject);
      }
    }
  }

private:

  std::function<void(dynamic_object<geometry_object_type>&, particle_object_type&)> m_callable;

};
