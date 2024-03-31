#pragma once

class particle_collision
{

public:

  particle_collision()
  {
  }

  auto operator()(auto&& geometryObject, auto&& particleObject, auto&& callable) -> void
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
        callable(geometryObject.Object(), particleObject);
      }
    }
  }

  auto operator()(auto&& geometryObject, std::ranges::input_range auto && particleObjectCollection, auto&& callable) -> void
  {
    std::for_each(std::execution::par, std::begin(particleObjectCollection), std::end(particleObjectCollection), [this,&geometryObject](auto& particleObject)
    {
      (*this)(geometryObject, particleObject);
    });
  }

  auto operator()(std::ranges::input_range auto&& geometryObjectCollection, std::ranges::input_range auto && particleObjectCollection) -> void
  {
    std::for_each(std::execution::par, std::begin(geometryObjectCollection), std::end(geometryObjectCollection), [this,&particleObjectCollection](auto& geometryObject)
    {
      if( !geometryObject->Destroyed() )
      {
        for( auto& particleObject : particleObjectCollection )
        {
          if( !particleObject.Destroyed() )
          {
            (*this)(geometryObject, particleObject, callable);
          }
        }
      }
    });
  }

private:

  std::mutex m_mutex;

};
