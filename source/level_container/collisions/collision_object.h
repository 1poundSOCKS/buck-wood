#pragma once

#include "objects/default_object.h"
#include "transformed_level_object_geometry.h"
#include "transformed_default_object_geometry.h"

class collision_object
{

public:

  collision_object(default_object& object) : m_object { object }, m_geometry { object }
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

  [[nodiscard]] auto Geometry() noexcept -> transformed_default_object_geometry&
  {
    return m_geometry;
  }

  [[nodiscard]] auto Geometry() const noexcept -> const transformed_default_object_geometry&
  {
    return m_geometry;
  }

private:

  default_object& m_object;
  transformed_default_object_geometry m_geometry;

};
