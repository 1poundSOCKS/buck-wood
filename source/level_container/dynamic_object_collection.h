#pragma once

#include "dynamic_object.h"

template <typename object_type>
class dynamic_object_collection
{

public:

  using dynamic_object_type = dynamic_object<object_type>;
  using collection_type = std::vector<dynamic_object_type>;

  template <typename...Args>auto Create(Args...args) -> void
  {
    m_objectCollection.emplace_back(std::forward<Args>(args)...);
  }

  template <typename...Args>auto Update(Args...args) -> void
  {
    for( auto& object : m_objectCollection )
    {
      object.Update(std::forward<Args>(args)...);
    }
  }

  [[nodiscard]] auto Size() const -> collection_type::size_type
  {
    return m_objectCollection.size();
  }

  [[nodiscard]] auto begin() const -> collection_type::const_iterator
  {
    return std::begin(m_objectCollection);
  }

  [[nodiscard]] auto end() const -> collection_type::const_iterator
  {
    return std::end(m_objectCollection);
  }

  [[nodiscard]] auto begin() -> collection_type::iterator
  {
    return std::begin(m_objectCollection);
  }

  [[nodiscard]] auto end() -> collection_type::iterator
  {
    return std::end(m_objectCollection);
  }

  auto EraseDestroyed() -> void
  {
    auto object = std::begin(m_objectCollection);

    while( object != std::end(m_objectCollection) )
    {
      object = object->Object().Destroyed() ? m_objectCollection.erase(object) : ++object;
    }
  }

  auto DoCollisionsWithGeometry(auto&& geometryObject, auto&& callable) -> void
  {
    for( auto& object : m_objectCollection )
    {
      if( object.Geometry().HasCollidedWith(geometryObject.Geometry()) )
      {
        callable(object.Object());
      }
    }
  }

  auto DoCollisionsWithGeometries(std::ranges::input_range auto&& geometryObjects, auto&& callable) -> void
  {
    for( auto& object1 : geometryObjects )
    {
      DoCollisionsWithGeometry(object1, [&object1, &callable](auto& object2) -> void
      {
        callable(object2, object1);
      });
    }
  }

  auto DoCollisionsWithPoint(auto&& pointObject, auto&& callable) -> void
  {
    for( auto& geometryObject : m_objectCollection )
    {
      if( have_geometry_and_point_collided(geometryObject, pointObject) )
      {
        callable(geometryObject.Object());
      }
    }
  }

  auto DoCollisionsWithPoints(std::ranges::input_range auto&& pointObjects, auto&& callable) -> void
  {
    for( auto& pointObject : pointObjects )
    {
      DoCollisionsWithPoint(pointObject, [&pointObject, &callable](auto& geometryObject) -> void
      {
        callable(geometryObject, pointObject);
      });
    }
  }

private:

  collection_type m_objectCollection;

};
