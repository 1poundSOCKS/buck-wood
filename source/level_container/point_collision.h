#pragma once

template <typename geometry_object_type, typename point_object_type>
class point_collision
{

public:

  point_collision(auto&& callable) : m_callable { callable }
  {
  }

  auto operator()(dynamic_object<geometry_object_type>& geometryObject, point_object_type& pointObject) -> void
  {
    BOOL collision = FALSE;
    HRESULT hr = geometryObject.Geometry().Get()->FillContainsPoint({pointObject.Position().x, pointObject.Position().y}, D2D1::Matrix3x2F::Identity(), &collision);

    if( SUCCEEDED(hr) && collision )
    {
      m_callable(geometryObject, pointObject);
    }
  }

  auto operator()(dynamic_object<geometry_object_type>& geometryObject, std::ranges::input_range auto && pointObjectCollection) -> void
  {
    for( auto& pointObject : pointObjectCollection )
    {
      (*this)(geometryObject, pointObject);
    }
  }

  auto operator()(dynamic_object_collection<geometry_object_type>& geometryObjectCollection, std::ranges::input_range auto && pointObjectCollection) -> void
  {
    for( auto& geometryObject : geometryObjectCollection )
    {
      for( auto& pointObject : pointObjectCollection )
      {
        (*this)(geometryObject, pointObject);
      }
    }
  }

private:

  std::function<void(dynamic_object<geometry_object_type>&, point_object_type&)> m_callable;

};
