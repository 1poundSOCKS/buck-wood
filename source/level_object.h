#pragma once

auto update_all(std::ranges::input_range auto&& objects, int64_t ticks)
{
  for( auto& object : objects )
  {
    object.Update(ticks);
  }
}

auto update_all(std::ranges::input_range auto&& objects, float interval)
{
  for( auto& object : objects )
  {
    object.Update(interval);
  }
}

auto erase_destroyed(std::ranges::input_range auto&& objects)
{
  auto object = std::begin(objects);

  while( object != std::end(objects) )
  {
    object = object->Destroyed() ? objects.erase(object) : ++object;
  }
}

[[nodiscard]] auto have_geometry_and_point_collided(auto& geometryObject, auto& pointObject) -> bool
{
  const auto& geometry = geometryObject.Geometry();
  const auto& point = pointObject.Position();

  BOOL collision = FALSE;
  HRESULT hr = geometry.Get()->FillContainsPoint({point.x, point.y}, D2D1::Matrix3x2F::Identity(), &collision);

  return SUCCEEDED(hr) && collision ? true : false;
}

[[nodiscard]] auto have_geometries_collided(auto& geometryObject1, auto& geometryObject2) -> bool
{
  const auto& geometry1 = geometryObject1.Geometry();
  const auto& geometry2 = geometryObject2.Geometry();

  D2D1_GEOMETRY_RELATION relation = D2D1_GEOMETRY_RELATION_UNKNOWN;
  HRESULT hr = geometry1.Get()->CompareWithGeometry(geometry2.Get(), D2D1::Matrix3x2F::Identity(), &relation);

  bool collided = false;

  if( SUCCEEDED(hr) )
  {
    switch( relation )
    {
      case D2D1_GEOMETRY_RELATION_IS_CONTAINED:
      case D2D1_GEOMETRY_RELATION_CONTAINS:
      case D2D1_GEOMETRY_RELATION_OVERLAP:
        collided = true;
        break;
    }
  }

  return collided;
}

auto do_geometries_to_points_collisions(std::ranges::input_range auto&& geometryObjects, std::ranges::input_range auto&& pointObjects, auto OnCollision) -> void
{
  std::for_each(std::begin(geometryObjects), std::end(geometryObjects), [&pointObjects, OnCollision](auto& geometryObject)
  {
    for( auto& pointObject : pointObjects )
    {
      {
        if( have_geometry_and_point_collided(geometryObject, pointObject) )
        {
          OnCollision(geometryObject, pointObject);
        }
      }
    }
  });
}

auto do_geometries_to_geometries_collisions(std::ranges::input_range auto&& geometryObjects1, std::ranges::input_range auto&& geometryObjects2, auto OnCollision) -> void
{
  std::for_each(std::begin(geometryObjects1), std::end(geometryObjects1), [&geometryObjects2, OnCollision](auto& geometryObject1)
  {
    for( auto& geometryObject2 : geometryObjects2 )
    {
      {
        if( have_geometries_collided(geometryObject1, geometryObject2) )
        {
          OnCollision(geometryObject1, geometryObject2);
        }
      }
    }
  });
}

auto do_geometry_to_geometries_collisions(auto& geometryObject, std::ranges::input_range auto&& geometryObjects, auto OnCollision) -> void
{
  for( auto& geometryObject2 : geometryObjects )
  {
    if( have_geometries_collided(geometryObject, geometryObject2) )
    {
      OnCollision(geometryObject, geometryObject2);
    }
  }
}
