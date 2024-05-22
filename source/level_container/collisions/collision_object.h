#pragma once

#include "objects/default_object.h"
#include "transformed_level_object_geometry.h"
// #include "transformed_default_object_geometry.h"

inline [[nodiscard]] auto ExtractGeometricObject(default_object& object) -> geometric_object&
{
  geometric_object* geometricObject { nullptr };
  std::visit([&geometricObject](auto& levelObject)
  {
    geometricObject = &levelObject;
  }, object.Get());

  return *geometricObject;
}

class collision_object
{

public:

  collision_object(default_object& object) : m_object { object }, m_geometry { ExtractGeometricObject(object) }
  {
  }

  [[nodiscard]] auto Object() noexcept -> default_object&
  {
    return m_object;
  }

  [[nodiscard]] auto Object() const noexcept -> const default_object&
  {
    return m_object;
  }

  [[nodiscard]] auto Geometry() noexcept -> transformed_level_object_geometry&
  {
    return m_geometry;
  }

  [[nodiscard]] auto Geometry() const noexcept -> const transformed_level_object_geometry&
  {
    return m_geometry;
  }

private:

  default_object& m_object;
  transformed_level_object_geometry m_geometry;

};
