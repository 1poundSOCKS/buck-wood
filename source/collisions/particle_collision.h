#pragma once

class particle_collision
{

public:

  particle_collision()
  {
  }

  auto operator()(auto&& particleObject, auto&& geometryObject, bool useGeometryBoundsOnly) -> void
  {
    auto particlePosition = particleObject.Position();
    auto geometryBounds = geometryObject.Bounds();

    if( particlePosition.x >= geometryBounds.left && particlePosition.x <= geometryBounds.right && 
        particlePosition.y >= geometryBounds.top && particlePosition.y <= geometryBounds.bottom )
    {
      BOOL collision = useGeometryBoundsOnly;
      HRESULT hr = useGeometryBoundsOnly ? S_OK : geometryObject.Geometry().Get()->FillContainsPoint({particleObject.Position().x, particleObject.Position().y}, D2D1::Matrix3x2F::Identity(), &collision);

      if( SUCCEEDED(hr) && collision )
      {
        std::lock_guard<std::mutex> guard(m_mutex);
        particleObject.Destroy();
      }
    }
  }

  auto operator()(std::ranges::input_range auto && particleObjectCollection, std::ranges::input_range auto&& geometryObjectCollection, bool useGeometryBoundsOnly) -> void
  {
    std::for_each(std::execution::par, std::begin(geometryObjectCollection), std::end(geometryObjectCollection), [this,&particleObjectCollection,useGeometryBoundsOnly](auto& geometryObject)
    {
      for( auto& particleObject : particleObjectCollection )
      {
        if( !particleObject.Destroyed() )
        {
          (*this)(particleObject, geometryObject, useGeometryBoundsOnly);
        }
      }
    });
  }

private:

  std::mutex m_mutex;

};
