#pragma once

#include "objects/default_object.h"
#include "level_object_geometry.h"

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

  // [[nodiscard]] auto Object() noexcept const -> const default_object&
  // {
  //   return m_object;
  // }

  [[nodiscard]] auto Geometry() noexcept -> level_object_geometry&
  {
    return m_geometry;
  }

  [[nodiscard]] auto Geometry() const noexcept -> const level_object_geometry&
  {
    return m_geometry;
  }

private:

  default_object& m_object;
  level_object_geometry m_geometry;

};
