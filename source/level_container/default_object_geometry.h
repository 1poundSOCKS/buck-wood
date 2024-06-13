#pragma once

#include "default_object.h"
#include "transformed_level_object_geometry.h"
#include "transformed_default_object_geometry.h"

class default_object_geometry
{

public:

  default_object_geometry(default_object& object) : m_object { object }, m_geometry { object }, m_bounds { direct2d::GetGeometryBounds(m_geometry.GetRaw()) }
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

  [[nodiscard]] auto Bounds() const noexcept -> RECT_F
  {
    return m_bounds;
  }

private:

  default_object& m_object;
  transformed_default_object_geometry m_geometry;
  RECT_F m_bounds;

};
